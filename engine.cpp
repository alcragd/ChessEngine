#include <iostream> 
#include <string>

using namespace std;


using ULL = unsigned long long;


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

    
    ULL getPieces(enumColor color, enumType type) const { return pieceBB[color] & pieceBB[type]; }

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

    bool isTheKingInCheck(enumColor color) {

    }

    // generar movimientos pseudo-legales para cada tipo de pieza por separado
    bool movePawn(int from, int to){}
    bool moveKnight(int from, int to){}
    bool moveBishop(int from, int to){}
    bool moveRook(int from, int to){}
    bool moveQueen(int from, int to){}
    bool moveKing(int from, int to){}

    // movimientos legales
    void makeMove(int from, int to){}


};


int main(){

    

}