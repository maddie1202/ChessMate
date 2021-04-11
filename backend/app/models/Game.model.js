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
    this.timeleft = game.timeleft;
};

Game.create = (newGame, result) => {
    const attributes = "difficulty, timeleft";
    const values = ""+ newGame.difficulty + ", "+ newGame.timeleft;
    sql.query("INSERT INTO Game(" + attributes + ") VALUES(" + values + ")", (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("created gameID: ", res.insertId);
        result(null, {"gameID" : res.insertId });
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
            console.log("found game details: ", res); // print gameID
            result(null, res);       //provide json
            return;
        }

        //not found Game with the gameID
        result({ kind: "not_found in db" }, null);
    });
};

//Retrieve latest game
//app.get("/getlatestgame/:userID", game.findLatest);
// get game details
Game.findLatest = (userID, result) => {
    sql.query("SELECT g.gameID FROM Game g, Results r WHERE r.gameID = g.gameID AND r.userID=\'" + userID + "\' AND r.result=-1 AND g.startDateTime >= ALL( SELECT Game.startDateTime FROM Game, Results WHERE Game.gameID = Results.gameID and userID = \'"+ userID + "\' AND result = -1)", (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found game details: ", res); // print game
            result(null, res);       //provide json
            return;
        }

        //not found latest Game with userID
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
        "UPDATE Game SET timeleft = "+ game.timeleft+ ", difficulty = " + game.difficulty +  " WHERE gameID = " + gameID,
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
            result(null, {"affectedRows": res.affectedRows});
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

    Game.getAll(userID, (err, data) => {
        if (!err){
            if(data.length >= 1){
                var num = data.length;
                var i;
                for(i = 0; i<num; i++){
                    const gameID = data[i].gameID;
                    Game.remove(gameID, (error, res) => {
                        if(err){
                            console.log(error);
                        }
                    });
                }
                const message = " " + num + " games deleted!";
                result({ "message" : message}, null)
            }
        }
        else{
            result(null, err);
        }
    });
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
