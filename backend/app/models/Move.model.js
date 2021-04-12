const sql = require("./db.js");

const Move =function(move){
    this.placements = move.placements;
};

//Add all possible move
Move.create = (move, result) => {

    var i;
    for(i = 0; i < move.length; i++){
        const attributes = "placements";
        const values = "\'"+move[i]+"\'";
        sql.query("INSERT INTO Move(" + attributes + ") VALUES(" + values + ")", (err, res) => {
            if(err){
                console.log("error: ", err);
                result(err, null);
                return;
            }
            console.log("created moveID: ", res.insertId);
        });
    }
   result(null, {message : "Done!"});

};

//Retrieve all possible moves
Move.getAll = (result) => {
    sql.query("SELECT * FROM Move", (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found a total of "+ res.length + ' moves');
            result(null, res);       //provide json
            return;
        }

        //not found moves
        result({ kind: "not_found in db" }, null);
    });
};


//delete all possible moves
Move.removeAll = (result) => {
    sql.query("DELETE FROM Move", (err, res) => {
        if(err){
            console.log("err from db: ", err);
            result(err, null);
            return;
        }

        if(res.affectedRows == 0){
            //not found any moves
            result({ kind: "not_found" }, null);
            return;
        }

        console.log("deleted moves, count = ", res.affectedRows);
        result(null, res);
    });
};

module.exports = Move;