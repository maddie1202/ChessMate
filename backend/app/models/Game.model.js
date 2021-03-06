// here we define constructor for "Game" object
// and use the db connection to write CRUD functions:
//
//- find a game with gameID
//- get all games for a userID
//- remove a game with gameID
//- remove all games with userID

const sql = require("./db.js");

//In MySQL, we have a table
//
//CREATE TABLE IF NOT EXISTS 'Game' (
//  gameID int(40) NOT NULL PRIMARY KEY AUTO_INCREMENT,
//  startDateTime DateTime NOT NULL,
// difficulty int(3)
//) ENGINE = InnoDB DEFAULT CHARSET = utf8;
//

const board = require("../controllers/board.controller.js");

const Game = function(game){
    this.gameID = game.gameID;
    this.startDateTime = game.startDateTime;
    this.difficulty = game.difficulty;
    this.boards = game.boards;
};

Game.create = (newGame, result) => {
    sql.query("INSERT INTO Game SET ?", newGame, (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created game: ", {id: res.insertId, ...newBoard });
        result(null, { id: res.insertId, ...newBoard});
    });
};

Game.findById = (gameID, result) => {


}

// There is a table in db called
// Results(userID, gameID, result)

Game.getAll = (userID, result) => {

}