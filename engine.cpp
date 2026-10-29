#include <bits/stdc++.h>

using namespace std;


using ULL = unsigned long long;

typedef struct Move {
    int from;
    int to;
} Move;


class CBoard {
    ULL pieceBB[8] = {0};
    bool whiteTurn = true;
    int castlingRights = 0;
    

    public:
        enum enumColor {
            nWhite, 
            nBlack,
            nEmpty = 8
        };
        enum enumType {   
            nPawn = 2,
            nKnight,
            nBishop,
            nRook,
            nQueen,
            nKing,
            nNone
        };
        enum enumCastlingR {
            WhiteShort = 1, // 0001
            WhiteLong = 2,  // 0010
            BlackShort = 4, // 0100
            BlackLong = 8   // 1000
        };
        

    
    void clearBoard(){
        for(auto &x : pieceBB) x = 0;
    }

    ULL getAllPieces() const { return pieceBB[nWhite] | pieceBB[nBlack]; }
    ULL getEmptySquares() const { return ~getAllPieces(); }
    ULL getWhitePieces() const { return pieceBB[nWhite]; }
    ULL getBlackPieces() const { return pieceBB[nBlack]; }
    bool canCastle(enumCastlingR cr) const { return ((castlingRights & cr) != 0); }
    void setCastleR(enumCastlingR cr) { castlingRights |= cr; }
    void removeCastleR(enumCastlingR cr) { castlingRights &= ~cr; }
    bool isWhiteTurn() const { return whiteTurn; }
    void switchTurn() { whiteTurn = (!whiteTurn); }
    enumColor getEnemyColor(enumColor mycol) const { return (mycol == nWhite ? nBlack : nWhite); }

    
    ULL getPieces(enumColor color, enumType type) const { return pieceBB[color] & pieceBB[type]; }

    ULL getPiecesColor(enumColor color) const { return (color == nWhite ? getWhitePieces() : getBlackPieces()); }

    enumColor getPieceColor(int square) const {
        if (square < 0 || square >= 64) return nEmpty;

        ULL mask = 1ULL << square;

        if (mask & getWhitePieces()) return nWhite;
        if (mask & getBlackPieces()) return nBlack;

        return nEmpty;
    }

    enumType getPieceType(int square) {
        if (square < 0 || square >= 64) return nNone;

        ULL mask = 1ULL << square;

        for(int i{nPawn} ; i <= nKing ; ++i) 
            if(pieceBB[i] & mask) return static_cast<enumType>(i);

        return nNone;
    }
    
    bool emptySquare(int square) {
        if (square < 0 || square >= 64) return false;

        return (getAllPieces() & (1ULL << square)) == 0;
    }

    void setPieces(enumColor color, enumType type, ULL bb) {
        ULL old = getPieces(color, type);

        pieceBB[color] &= ~old;
        pieceBB[type] &= ~old;

        pieceBB[color] |= bb;
        pieceBB[type] |= bb;
    }

    void addPiece(enumColor color, enumType type, int square) {
        if (square < 0 || square >= 64) return;
        ULL bb = getPieces(color, type);
        
        bb |= 1ULL << square;

        setPieces(color, type, bb);
    }

    void removePiece(int square) { 
        if (square < 0 || square >= 64) return;

        for(auto &x : pieceBB)
            x &= ~(1ULL << square); 
    }

    bool movePiece(int from, int to){
        if(from < 0 || from >= 64 || to < 0 || to >= 64 || to == from || emptySquare(from)) return false;

        enumType type = getPieceType(from);
        enumColor color = getPieceColor(from);

        if(color == getPieceColor(to)) return false;

        if(!emptySquare(to)) removePiece(to);
        removePiece(from);
        addPiece(color, type, to);

        return true;
    }
    
    void setStartingPos() {
        clearBoard();

        whiteTurn = true;
        castlingRights = 15; // 1111

        addPiece(nWhite, nRook,   0);  // a1
        addPiece(nWhite, nKnight, 1);  // b1
        addPiece(nWhite, nBishop, 2);  // c1
        addPiece(nWhite, nQueen,  3);  // d1
        addPiece(nWhite, nKing,   4);  // e1
        addPiece(nWhite, nBishop, 5);  // f1
        addPiece(nWhite, nKnight, 6);  // g1
        addPiece(nWhite, nRook,   7);  // h1

        for (int i = 8; i < 16; i++) {
            addPiece(nWhite, nPawn, i); // a2-h2
        }

        addPiece(nBlack, nRook,   56); // a8
        addPiece(nBlack, nKnight, 57); // b8
        addPiece(nBlack, nBishop, 58); // c8
        addPiece(nBlack, nQueen,  59); // d8
        addPiece(nBlack, nKing,   60); // e8
        addPiece(nBlack, nBishop, 61); // f8
        addPiece(nBlack, nKnight, 62); // g8
        addPiece(nBlack, nRook,   63); // h8

        for (int i = 48; i < 56; i++) {
            addPiece(nBlack, nPawn, i); // a7-h7
        }
    }


    ULL getKnightAttackMask(int square) {
        if (square < 0 || square >= 64) return 0;
        ULL mask = 0;

        static constexpr array<pair<int, int>, 8> Kn_mov = {{
            {2, 1}, {2, -1}, {1, 2}, {1, -2},
            {-2, 1}, {-2, -1}, {-1, 2}, {-1, -2}
        }};

        int row = square/8;
        int col = square%8;

        for(auto &[x,y] : Kn_mov){
            if(row + x >= 0 && row + x <= 7 && col + y >= 0 && col + y <= 7){
                mask |= (1ULL << (col + y + (row + x)*8));
            }
        }
    
        return mask;
    }

    ULL getPawnAttackMask(int square, enumColor color) {
        if (square < 0 || square >= 64 || color == nEmpty) return 0;

        ULL mask = 0;

        int row = square/8;
        int col = square%8;

        int k = (color == nWhite ? 1 : -1);

        if(row + k >= 0 && row + k <= 7 && col + k >= 0 && col + k <= 7)
            mask |= (1ULL << (col + k + (row + k)*8));
        if(row + k >= 0 && row + k <= 7 && col - k >= 0 && col - k <= 7)
            mask |= (1ULL << (col - k + (row + k)*8));
        
        return mask;
    }

    ULL getKingAttackMask(int square){
        if (square < 0 || square >= 64) return 0;

        ULL mask = 0;

        int row = square/8;
        int col = square%8;

        static constexpr array<pair<int, int>, 8> K_mov = {{
            {1, 0}, {0, 1}, {1, 1}, {0, -1},
            {-1, 0}, {1, -1}, {-1, 1}, {-1, -1}
        }};

        for(auto &[x,y] : K_mov){
            if(row + x <= 7 && row + x >= 0 && col + y <= 7 && col + y >= 0)
                mask |= (1ULL << (col + y + (row + x)*8));
        }

        return mask;
    }

    ULL getBishopAttackMask(int square, enumColor color){
        if (square < 0 || square >= 64) return 0;

        ULL mask = 0;

        int row = square/8;
        int col = square%8;

        static constexpr array<pair<int, int>, 4> B_mov = {{
            {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
        }};

        // la ultima pieza que ataca un alfil es el borde del tablero, una casilla ANTES de una pieza del mismo color 
        // o la primer pieza enemiga que encuentre

        int k;
        ULL mask_enemy_pieces = getPiecesColor(getEnemyColor(color));
        ULL mask_all_pieces = getAllPieces();
        for(auto &[x,y] : B_mov){
            k = 1;
            int tmppos = (col + y*k + (row + x*k)*8);
            while(row + x*k <= 7 && row + x*k >= 0 &&
                col + y*k <= 7 && col + y*k >= 0 && 
                (mask_all_pieces & (1ULL << tmppos)) == 0)
            {
                mask |= (1ULL << tmppos);
                k++;
                tmppos = (col + y*k + (row + x*k)*8);
            }
            tmppos = (col + y*k + (row + x*k)*8);
            if(row + x*k <= 7 && row + x*k >= 0 &&
                col + y*k <= 7 && col + y*k >= 0 && 
                (mask_enemy_pieces & (1ULL << tmppos)) != 0) 
                    mask |= (1ULL << tmppos); 
        }

        return mask;
    }

    ULL getRookAttackMask(int square, enumColor color){
        if (square < 0 || square >= 64) return 0;

        ULL mask = 0;

        int row = square/8;
        int col = square%8;

        static constexpr array<pair<int, int>, 4> R_mov = {{
            {1, 0}, {0, 1}, {0, -1}, {-1, 0}
        }};

        // la ultima pieza que ataca una torre es el borde del tablero, una casilla ANTES de una pieza del mismo color 
        // o la primer pieza enemiga que encuentre

        int k;
        ULL mask_enemy_pieces = getPiecesColor(getEnemyColor(color));
        ULL mask_all_pieces = getAllPieces();
        for(auto &[x,y] : R_mov){
            k = 1;
            int tmppos = (col + y*k + (row + x*k)*8);
            while(row + x*k <= 7 && row + x*k >= 0 &&
                col + y*k <= 7 && col + y*k >= 0 && 
                (mask_all_pieces & (1ULL << tmppos)) == 0)
            {
                mask |= (1ULL << tmppos);
                k++;
                tmppos = (col + y*k + (row + x*k)*8);
            }
            tmppos = (col + y*k + (row + x*k)*8);
            if(row + x*k <= 7 && row + x*k >= 0 &&
                col + y*k <= 7 && col + y*k >= 0 && 
                (mask_enemy_pieces & (1ULL << tmppos)) != 0) 
                    mask |= (1ULL << tmppos); 
        }

        return mask;
    }

    ULL getQueenAttackMask(int square, enumColor color) {
        return (getBishopAttackMask(square, color) | getRookAttackMask(square, color));
    }

    bool isTheKingInCheck(enumColor color) {
        ULL mask_king = getPieces(color,nKing);
        if(mask_king == 0) return false;

        int pos = __builtin_ctzll(mask_king);

        enumColor enemyCol = getEnemyColor(color);

        if(getKnightAttackMask(pos) & getPieces(enemyCol,nKnight)) return true;

        if(getPawnAttackMask(pos, color) & getPieces(enemyCol,nPawn)) return true;
        
        if(getKingAttackMask(pos) & getPieces(enemyCol, nKing)) return true;
        
        ULL mask_enemy_queen = getPieces(enemyCol, nQueen);
        ULL mask_rook = getRookAttackMask(pos, color);
        if(mask_rook & getPieces(enemyCol, nRook)) return true;
        if(mask_rook & mask_enemy_queen) return true;

        ULL mask_bishop = getBishopAttackMask(pos, color);
        if(mask_bishop & getPieces(enemyCol, nBishop)) return true;
        if(mask_bishop & mask_enemy_queen) return true;

        return false;
    }

    // generar movimientos pseudo-legales para cada tipo de pieza por separado
    bool movePawn(int from, int to){}
    bool moveKnight(int from, int to){}
    bool moveBishop(int from, int to){}
    bool moveRook(int from, int to){}
    bool moveQueen(int from, int to){}
    bool moveKing(int from, int to){}

    // movimientos legales
    vector<Move> generateLegalMoves() {
        vector<Move> moves;


    }

    void makeMove(int from, int to){}


};


int main(){

    

}