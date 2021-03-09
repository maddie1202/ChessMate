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
        console.log("created game: ", {id: res.insertId, ...newGame });
        result(null, { id: res.insertId, ...newGame});
    });
};

// get game details
Game.findById = (gameID, result) => {
    sql.query("SELECT * FROM Game WHERE gameID=" + gameID, (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found game details: ", res[0]); // print gameID
            result(null, res);       //provide json
            return;
        }

        //not found Game with the gameID
        result({ kind: "not_found in db" }, null);
    });
};

// There is a table in db called
// Results(userID, gameID, result)

// get all games played by a user
Game.getAll = (userID, result) => {
    sql.query("SELECT * FROM Results WHERE userID=" + userID, (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found a total of "+ res.length + ' games');
            result(null, res);       //provide json
            return;
        }

        //not found games for user with userID
        result({ kind: "not_found in db" }, null);
    });
}

module.exports = Game;