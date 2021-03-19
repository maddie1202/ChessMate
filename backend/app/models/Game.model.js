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
    this.difficulty = game.difficulty;
};

Game.create = (newGame, result) => {
    const attributes = "gameID, difficulty";
    const values = ""+ newGame.gameID + ", " + newGame.difficulty + "";
    sql.query("INSERT INTO Game(" + attributes + ") VALUES(" + values + ")", (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created game: ", {...newGame });
        result(null, {...newGame});
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
    sql.query("SELECT * FROM Results WHERE userID= \"" + userID + "\"", (err, res) => {
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

//update game with gameID and game details
Game.updateById = (gameID, game, result) => {
    sql.query(
        "UPDATE Game SET difficulty = " + game.difficulty+ " WHERE gameID = " + gameID,
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0) {
                //not found game with gameId
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("updated game: ", { ...game });
            result(null, { ...game});
        }
    );
};

//remove a game using gameID
Game.remove = (gameID, result) => {
    sql.query(
        "DELETE FROM Game WHERE gameID = " + gameID,
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0){
                //not found game with gameId
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("deleted game with gameID: ", gameID);
            result(null, res);
        }
    );

};

//remove all games with userID
Game.removeAll = (userID, result) => {
    sql.query(
        "DELETE FROM Result WHERE userID = \"" + userID + "\"",
        (err, res) => {
            if(err){
                console.log("error: ", err);
                result(null, err);
                return;
            }

            console.log('deleted ' + res.affectedRows + 'games');
            result(null, res);
        }
    );
};

Game.createResult = (userID, gameID, resultnum, result) => {

        const attributes = "userID, gameID, result";
        const values = "\""+ userID + "\", " + gameID + ", " + resultnum;
        sql.query("INSERT INTO Results(" + attributes + ") VALUES(" + values + ")", (err, res) => {
            if(err){
                console.log("error: ", err);
                result(err, null);
                return;
            }
            const user = "" + userID + "";
            var resultobj = {"userID" : user,
                              "gameID" : gameID,
                              "result" : resultnum };

            console.log("created result: ", {resultobj});
            result(null, {resultobj});
        });
};

Game.updateResult = (userID, gameID, resultnum, result) => {
    sql.query(
        "UPDATE Results SET result = " +resultnum+ " WHERE userID = \"" + userID + "\" AND gameID = " + gameID,
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0) {
                //not found result with userID gameID
                result({ kind: "not_found" }, null);
                return;
            }

            const user = "" + userID + "";
            var resultobj = {"userID" : user,
                              "gameID" : gameID,
                              "result" : resultnum };

            console.log("updated result: ", {...resultobj});
            result(null, {...resultobj});
        }
    );
};

module.exports = Game;