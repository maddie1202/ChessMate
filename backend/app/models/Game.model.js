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
    const attributes = "difficulty";
    const values = ""+ newGame.difficulty + "";
    sql.query("INSERT INTO Game(" + attributes + ") VALUES(" + values + ")", (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created game: ", res);
        result(null, res);
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

            console.log("updated game: ",res);
            result(null, res);
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

            console.log("created result: ", resultobj);
            result(null, resultobj);
        });

        //add this result to Achievements if result = 1
        if(resultnum == 1){
        //TODO:
            sql.query("SELECT difficulty FROM Game WHERE gameID = " + gameID, (err, res) => {
                if(err){
                    console.log("err in select difficulty: ", err);
                    return;
                }

                var difficulty = res[0].difficulty;

                console.log("diff = " + difficulty);
                sql.query("SELECT realCount FROM Achievements WHERE userID = \"" + userID + "\" AND difficulty = "+difficulty, (err, res) => {
                    if(err){
                        console.log("err in select realCount: ", err);
                        return;
                    }
                    if(res.length >= 1){
                        var num = res.length;
                        var realCount = res;
                        var i;
                        for(i = 0; i<num; i++){
                            var count = realCount[i].realCount + 1;
                            var value = "realCount="+count;
                            var condition = "userID=\""+userID+"\" AND difficulty="+difficulty;
                            sql.query("UPDATE Achievements SET "+value+ " WHERE "+condition);
                        }
                    }
                });
            });

        }
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

        console.log("updated result: ", resultobj);
        result(null, resultobj);
    });

    //add this result to Achievements if result = 1
    if(resultnum == 1 || resultnum == 0){

        sql.query("SELECT difficulty FROM Game WHERE gameID = " + gameID, (err, res) => {
            if(err){
                console.log("err in select difficulty: ", err);
                return;
            }

            var difficulty = res[0].difficulty;

            console.log("diff = " + difficulty);
            sql.query("SELECT realCount FROM Achievements WHERE userID = \"" + userID + "\" AND difficulty = "+difficulty, (err, res) => {
                if(err){
                    console.log("err in select realCount: ", err);
                    return;
                }
                if(res.length >= 1){
                    var num = res.length;
                    var realCount = res;
                    var i;
                    for(i = 0; i<num; i++){
                        var count;
                        if(resultnum == 0){
                            count = realCount[i].realCount - 1;
                        }
                        else {
                            count = realCount[i].realCount + 1;
                        }

                        var value = "realCount="+count;
                        var condition = "userID=\""+userID+"\" AND difficulty="+difficulty;
                        sql.query("UPDATE Achievements SET "+value+ " WHERE "+condition);
                    }
                }
            });
        });
    }

};

module.exports = Game;