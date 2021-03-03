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
//CREATE TABLE IF NOT EXISTS 'boards' (
//  board_id int(40) NOT NULL PRIMARY KEY AUTO_INCREMENT,
//  placements varchar(255) NOT NULL, /////////NEED TO CREATE A DIFFERENT TABLE HERE
//  game_id int(20) NOT NULL
//) ENGINE = InnoDB DEFAULT CHARSET = utf8;
//



//constructor
const Board = function(board){
    this.board_id = board.board_id;
    this.placements = board.placements;
    this.game_id = board.game_id; // unique
};

Board.create = (newBoard, result) => {
    sql.query("INSERT INTO boards SET ?", newBoard, (err,res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created board: ", {id: res.insertId, ...newBoard });
        result(null, { id: res.insertId, ...newCustomer});
    });
};

Board.findById = (boardId, result) => {
    sql.query('SELECT * FROM Customers WHERE id = ${boardId}', (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found board: ", res[0]); // print boardId
            result(null, res[1]);       //provide placements
            return;
        }

        //not found Board with the boardId
        result({ kind: "not_found" }, null);
    });
};

Board.getAll = (gameId, result) => {
    sql.query("SELECT * FROM boards", (err, res) => {
        if(err) {
            console.log("error: ", err);
            result(null, err);
            return;
        }

        console.log("boards: ", res);
        result(null, res);
    });
};

Board.updateById = (boardId, board, result) => {
    sql.query(
        "UPDATE boards SET placements = ?, game_id = ? WHERE board_id = ?",
        [boardId, board.placements, board.game_id],
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

            console.log("updated board: ", { id: boardId, ...board });
            result(null, { id: boardId, ...board });
        }
    );
};


Board.remove = (boardId, result) => {
    sql.query(
        "DELETE FROM boards WHERE board_id = ?", boardId,
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

            console.log("deleted board with board_id: ", boardId);
            result(null, res);
        }
    );
};

Board.removeAll = (gameId, result) => {
    sql.query(
        "DELETE FROM boards WHERE game_id = ?", gameId,
        (err, res) => {
            if(err){
                console.log("error: ", err);
                result(null, err);
                return;
            }

            console.log('deleted ${res.affectedRows} boards');
            result(null, res);
        }
    );
};

module.exports = Board;
