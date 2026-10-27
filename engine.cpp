#include <iostream> 
#include <string>

using namespace std;


using ULL = unsigned long long;


class CBoard {
    ULL pieceBB[8] = {0};  

    public:
        enum enumColor {
            nWhite, 
            nBlack
        };
        enum enumType {   
            nPawn = 2,
            nKnight,
            nBishop,
            nRook,
            nQueen,
            nKing
        };
        

    
    void clearBoard(){
        for(auto &x : pieceBB) x = 0;
    }

    ULL getAllPieces() const { return pieceBB[nWhite] | pieceBB[nBlack]; }
    ULL getEmptySquares() const { return ~getAllPieces(); }
    ULL getWhitePieces() const { return pieceBB[nWhite]; }
    ULL getBlackPieces() const { return pieceBB[nBlack]; }
    
    ULL getPieces(enumColor color, enumType type) const { return pieceBB[color] & pieceBB[type]; }
    

    void setPieces(enumColor color, enumType type, ULL bb) {
        pieceBB[color] &= ~getPieces(color, type);
        pieceBB[type] &= ~getPieces(color, type);

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

        for(auto &x  : pieceBB){
            x &= ~(1ULL << square);
        }
        
    }
    void movePiece(int from, int to){ /* :skull: */ }
    

    void setStartingPos() {
        clearBoard();

        // Piezas blancas
        addPiece(nWhite, nRook,   0);  // a1
        addPiece(nWhite, nKnight, 1);  // b1
        addPiece(nWhite, nBishop, 2);  // c1
        addPiece(nWhite, nQueen,  3);  // d1
        addPiece(nWhite, nKing,   4);  // e1
        addPiece(nWhite, nBishop, 5);  // f1
        addPiece(nWhite, nKnight, 6);  // g1
        addPiece(nWhite, nRook,   7);  // h1

        // Peones blancos
        for (int i = 8; i < 16; i++) {
            addPiece(nWhite, nPawn, i); // a2-h2
        }

        // Piezas negras
        addPiece(nBlack, nRook,   56); // a8
        addPiece(nBlack, nKnight, 57); // b8
        addPiece(nBlack, nBishop, 58); // c8
        addPiece(nBlack, nQueen,  59); // d8
        addPiece(nBlack, nKing,   60); // e8
        addPiece(nBlack, nBishop, 61); // f8
        addPiece(nBlack, nKnight, 62); // g8
        addPiece(nBlack, nRook,   63); // h8

        // Peones negros
        for (int i = 48; i < 56; i++) {
            addPiece(nBlack, nPawn, i); // a7-h7
        }
}

    

};


int main(){

    

}