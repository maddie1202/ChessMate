
//This model will be responsible for:
// - find all achievements for a user
// - find goal by goalID

const sql = require("./db.js");

//constructor
const Ach = function(goal){
    this.gameCount = goal.gameCount;
    this.gameDifficulty = goal.gameDifficulty;
};


//There is a table in DB called
//Achievements(userID, goalID, difficulty, reqCount, realCount)
// and another called
//GoalsLookup(goalID, gameCount, gameDifficulty)
Ach.getAllGoals = (result) => {
    sql.query("SELECT * FROM GoalsLookup", (err, res) => {
        if(err) {
            console.log("error: ", err);
            result(null, err);
            return;
        }

        console.log("goals: ", res);
        result(null, res);
    });
};

Ach.getAllGoalsStatus = (userID, result) => {

    sql.query("SELECT * FROM Achievements WHERE userID =\"" + userID + "\"", (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        if(res.length >= 1){
            console.log("goals status : ", res);
        }
         result(null, res);
    });

};

Ach.getAllCompleted = (userID, result) => {
    sql.query("SELECT * FROM Achievements WHERE userID = \"" + userID + "\" AND reqCount <= realCount", (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }
        console.log("achievements : ", res);
        result(null, res);
    });
};

module.exports = Ach;