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
};

//create a new board in table
Board.create = (newBoard, result) => {

    const attributes = "boardID, placements, gameID, sequenceNumber";
    const values = "" + newBoard.boardID + ", \"" + newBoard.placements + "\", " + newBoard.gameID + ", " + newBoard.sequenceNum + "";

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
