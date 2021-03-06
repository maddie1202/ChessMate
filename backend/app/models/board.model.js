//here we define constructor for "Board" object here
// and use the db connection to write CRUD functions:
//
//- create a new board
//- find a board by board id
//- get all boards for a game id
//- update a board by id
//- remove a board by board id
//- remove all boards for a game id

const sql = require("./db.js");

//In MySQL, we have a table
//
//CREATE TABLE IF NOT EXISTS 'Board' (
//  boardID int(40) NOT NULL PRIMARY KEY AUTO_INCREMENT,
//  placements varchar(1300) NOT NULL,
//  gameID int(20) NOT NULL
//  sequenceNum int(20)
//) ENGINE = InnoDB DEFAULT CHARSET = utf8;

// Test boards
const initialLayout = ['-9', '-19', '-29', '-48', '-39', '-30', '-20', '-10',
                        '0', '0', '0', '0', '0', '0', '0', '0',
                        '0', '0', '0', '0', '0', '0', '0', '0',
                        '0', '0', '0', '0', '0', '0', '0', '0',
                        '0', '0', '0', '0', '0', '0', '0', '0',
                        '1', '2', '3', '4', '5', '6', '7', '8',
                        '9', '19', '29', '48', '39', '30', '20', '10'];

const firstMove = ['-9', '-19', '-29', '-48', '-39', '-30', '-20', '-10',
                          '0', '0', '0', '0', '0', '0', '0', '0',
                          '0', '0', '0', '0', '0', '0', '0', '0',
                          '0', '0', '0', '0', '5', '0', '0', '0',
                          '0', '0', '0', '0', '0', '0', '0', '0',
                          '1', '2', '3', '4', '0', '6', '7', '8',
                          '9', '19', '29', '48', '39', '30', '20', '10'];

//constructor
const Board = function(board){
    this.boardID = board.boardID;
    this.placements = board.placements;
    this.gameID = board.gameID;
    this.sequenceNum = board.sequenceNum;
    this.wrookO_moved = board.wrookO_moved,
    this.wrookI_moved = board.wrookI_moved,
    this.brookO_moved = board.brookO_moved,
    this.brookI_moved = board.brookI_moved,
    this.wking_moved = board.wking_moved,
    this.bking_moved = board.bking_moved
};

//create a new board in table
Board.create = (newBoard, result) => {

    const attributes = "placements, gameID, sequenceNumber,wrookO_moved, wrookI_moved, brookO_moved, brookI_moved, wking_moved, bking_moved";
    const values =  "\"" + newBoard.placements + "\", " + newBoard.gameID + ", " + newBoard.sequenceNum + ", " + newBoard.wrookO_moved + ", " + newBoard.wrookI_moved + ", " + newBoard.brookO_moved + ", " + newBoard.brookI_moved + ", " + newBoard.wking_moved + ", " + newBoard.bking_moved;

    sql.query("INSERT INTO Board(" + attributes + ") VALUES(" + values + ")", (err,res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created board: ", {id: res.insertId, ...newBoard });
        result(null, { id: res.insertId, ...newBoard});
    });
};

//find board by boardID
Board.findById = (boardID, result) => {
    sql.query('SELECT * FROM Board WHERE boardID='+ boardID, (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found board: ", res[0]); // print boardID
            result(null, res);       //provide json
            return;
        }

        //not found Board with the boardID
        result({ kind: "not_found in db" }, null);
    });
};


//Retrive latest board with gameID
//app.get("/getlatestboard/:gameID", board.findLatest);
Board.findLatest = (gameID, result) => {
    sql.query("SELECT * FROM Board WHERE gameID = " + gameID + " AND sequenceNumber >= ALL (SELECT MAX(sequenceNumber) FROM Board WHERE gameID = " + gameID + " GROUP BY gameID)", (err, res) => {
        if(err) {
            console.log("error: ", err);
            result(null, err);
            return;
        }

        console.log("board: ", res);
        result(null, res[0]);
    });
};


//find all boards with gameID
Board.getAll = (gameID, result) => {
    sql.query("SELECT * FROM Board WHERE gameID=" + gameID, (err, res) => {
        if(err) {
            console.log("error: ", err);
            result(null, err);
            return;
        }

        console.log("boards: ", res);
        result(null, res);
    });
};

//update board with boardID and board details
Board.updateById = (boardID, board, result) => {
    sql.query(
        "UPDATE Board SET placements = ?, gameID = ?, sequenceNumber = ? WHERE boardID = ?",
        [board.placements, board.gameID, board.sequenceNum, boardID],
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0) {
                //not found board with boardId
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("updated board: ", { id: boardID, ...board });
            result(null, { id: boardID, ...board });
        }
    );
};

//remove a board using boardID
Board.remove = (boardID, result) => {
    sql.query(
        "DELETE FROM Board WHERE boardID = " + boardID,
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0){
                //not found board with boardId
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("deleted board with boardID: ", boardID);
            result(null, res);
        }
    );
};

//remove all boards with gameID
Board.removeAll = (gameID, result) => {
    sql.query(
        "DELETE FROM Board WHERE gameID = " + gameID,
        (err, res) => {
            if(err){
                console.log("error: ", err);
                result(null, err);
                return;
            }

            console.log('deleted ' + res.affectedRows + 'boards');
            result(null, res);
        }
    );
};

module.exports = Board;
