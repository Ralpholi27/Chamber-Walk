#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    std::ifstream inputFile;
    bool doesNotHaveExit = true;
    inputFile.open(fileName);
    if(!inputFile.is_open()){ // checks if file opens
        return nullptr;
    }

    // max row 
    inputFile >> maxRow;
    if(inputFile.fail()){
        return nullptr;
    }
    if((maxRow > 999999) || (maxRow <= 0)){
        return nullptr;
    }
  

    // max column 
    inputFile >> maxCol;
    if(inputFile.fail()){
        return nullptr;
    }
    if((maxCol > 999999) || (maxCol <= 0)){
        return nullptr;
    }

    if(maxRow > (INT32_MAX/maxCol)){
        return nullptr;
    }

    //row of player
    inputFile >> player.row;
    if(inputFile.fail()){
        return nullptr;
    }
    if((player.row < 0) || (player.row >= maxRow)){
        return nullptr;
    }

    // column of player
    inputFile >> player.col;
    if(inputFile.fail()){
        return nullptr;
    }
    if(player.col < 0 || player.col >= maxCol){
        return nullptr;
    }

  

    char **map = createMap(maxRow, maxCol);
    if(map == nullptr){
        return nullptr;
    }
    for(int r = 0; r < maxRow; r++){
        for(int c = 0; c < maxCol; c++){
            inputFile >> map[r][c];
            if(inputFile.fail()){
                deleteMap(map,maxRow);
                return nullptr;
            }
            if(((map[r][c] != 'o') && (map[r][c] != '-') && (map[r][c] != '$') && (map[r][c] != '@') && (map[r][c] != 'M') && (map[r][c] != '+') && (map[r][c] != '?') && (map[r][c] != '!'))){
                deleteMap(map,maxRow);
                return nullptr;
            }
            if((map[r][c] == '!') || (map[r][c] == '?')){
                doesNotHaveExit = false;
            }
        }
    }
    if(doesNotHaveExit){
        deleteMap(map,maxRow);
        return nullptr;
    }
    if((map[player.row][player.col] == '!') || (map[player.row][player.col] == '?') || (map[player.row][player.col] == '+')){
        deleteMap(map,maxRow);
        return nullptr;
    }
    map[player.row][player.col] = TILE_PLAYER;

    char order;
    inputFile >> order;
    if(!inputFile.fail()){
    deleteMap(map,maxRow);
    return nullptr;
}
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input == MOVE_DOWN){
        nextRow += 1;
    }
    else if(input == MOVE_UP){
        nextRow -= 1;
    }
    else if(input == MOVE_LEFT){
        nextCol -= 1;
    }
    else if(input == MOVE_RIGHT){
        nextCol += 1;
    }
    else{
        return ;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {

    char **grid = new char* [maxRow];
    if((maxRow > 999999) || (maxRow <= 0)){
        return nullptr;
    }
    if((maxCol > 999999) || (maxCol <= 0)){
        return nullptr;
    }


    for(int r = 0; r< maxRow; r++){
        grid[r] = new char[maxCol];
        for(int c = 0; c < maxCol; c++ ){
            grid[r][c] = TILE_OPEN;
        }
    }

    return grid;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map == nullptr){
        maxRow = 0;
    }
    for(int r = 0; r < maxRow; r++){
        delete[] map[r];
    }

    delete[] map;
    map = nullptr;
    maxRow = 0;
    }

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    bool valid = true;
    if(map == nullptr){
        return nullptr;
    }
    if(maxRow <= 0){
        return nullptr;
    }
    if(maxCol <= 0){
        return nullptr;
    }
    char **mapResize = createMap(maxRow * 2, maxCol * 2);

    int newRow = maxRow * 2;
    int newCol = maxCol * 2;

    for( int f = 0; f < maxRow * 2; f++){
        for( int b = 0; b < maxCol * 2; b++){
           if(map[f%maxRow][b%maxCol] == TILE_PLAYER) {
                if(valid){
                    mapResize[f][b] = TILE_PLAYER;
                }else{
                    mapResize[f][b] = TILE_OPEN;
                }
                valid = false;
                continue;
            }

            mapResize[f][b] = map[f%maxRow][b%maxCol];
            
        }
    }

    deleteMap(map,maxRow);

    maxRow = newRow;
    maxCol = newCol;
    return mapResize;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    int status;
     if(map == nullptr){
        return STATUS_STAY;
     }
     if((nextRow < 0) || (nextCol < 0)){
        return STATUS_STAY;
     }
     if((nextRow >= maxRow) || (nextCol >= maxCol)){
        return STATUS_STAY;
     }
     if(map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER){
        return STATUS_STAY;
     }

     if(map[nextRow][nextCol] == TILE_OPEN){
        status = STATUS_MOVE;
     }
     if(map[nextRow][nextCol] == TILE_TREASURE){
        player.treasure += 1;
        status = STATUS_TREASURE;
     }
     if(map[nextRow][nextCol] == TILE_AMULET){
        status = STATUS_AMULET;
     }
     if(map[nextRow][nextCol] == TILE_DOOR){
        status = STATUS_LEAVE;
     }
     if(map[nextRow][nextCol] == TILE_EXIT){
        if(player.treasure > 0){
        status = STATUS_ESCAPE;
        }else{
            return STATUS_STAY;
        }
     }

    map[player.row][player.col] = TILE_OPEN;
    map[nextRow][nextCol] = TILE_PLAYER;

    player.row = nextRow;
    player.col = nextCol;
    
    return status;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    if(map == nullptr){
        return false;
    }
    bool status = false;

    //checks left of player
    for(int c = player.col - 1; c >= 0; c--){
        if(map[player.row][c] == TILE_PILLAR){
            break;
        }
        if(map[player.row][c] == TILE_MONSTER){
            map[player.row][c] = TILE_OPEN;
            map[player.row][c+1] = TILE_MONSTER;
        }
        
    }

    //checks right
    for(int c = player.col + 1; c < maxCol; c++){
        if(map[player.row][c] == TILE_PILLAR){
            break;
        }
        if(map[player.row][c] == TILE_MONSTER){
            map[player.row][c] = TILE_OPEN;
            map[player.row][c-1] = TILE_MONSTER;
        }
    }

    //checks down
    for(int r = player.row - 1; r >= 0; r--){
        if(map[r][player.col] == TILE_PILLAR){
            break;
        }
        if(map[r][player.col] == TILE_MONSTER){
            map[r][player.col] = TILE_OPEN;
            map[r+1][player.col] = TILE_MONSTER;
        }
    }

    //checks up
    for(int r = player.row + 1; r < maxRow; r++){
        if(map[r][player.col] == TILE_PILLAR){
           return status;
        }
        if(map[r][player.col] == TILE_MONSTER){
            map[r][player.col] = TILE_OPEN;
            map[r-1][player.col] = TILE_MONSTER;
        }
    }
    if(map[player.row][player.col] == TILE_MONSTER){
        status = true;
    }
    return status;
}
