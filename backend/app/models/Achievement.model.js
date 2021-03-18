
//This model will be responsible for:
// - find all achievements for a user
// - find goal by goalID

const sql - require("./db.js");

//constructor
const Goal = function(goal){
    this.goalID = goal.goalID;
    this.gameCount = goal.gameCount;
    this.gameDifficulty = goal.gameDifficulty;
};


//There is a table in DB called
//Achievements(userID, goalID)
// and another called
//GoalsLookup(goalID, gameCount, gameDifficulty)

Goal.findAll = (userID, result) => {
    sql.query('SELECT goalID FROM Achievements WHERE userID = \" ${userID} \" ', (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found achievements: ", res); // print goalID's

            const totalAchievements = res.length;
            var i;
            for (i = 0; i < totlAchievements; i++) {
                goalID = res[i];
                //TODO: use helper function Goal.findById
            }

            //result(null, res[1]); //provide placements
            return;
        }
    });

};

//GoalsLookup(goalID, gameCount, gameDifficulty)
Goal.findById = (goalID, result) => {
    sql.query('SELECT * FROM GoalsLookup WHERE goalID = ${goalID}', (err, res) => {
        if(err){
            console.log("error: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found goal: ", res[0]); // print goalID
            result(null, res);       //provide goal info
            return;
        }

        //not found goal with the goalID
        result({ kind: "not_found" }, null);
    });
};