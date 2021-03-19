/*
 Here we define constructor for "User" object here
 and use the db connection to write CRUD functions:

 - create a new user
 - find a user by userId
 - update a user by userId
 - remove a user by userId
*/
// test push from server

const sql = require("./db.js");

/*
 * userId: a user's unique id
 * name: a user's name
 */
const User = function(user) {
  this.userID = user.userID;
  this.name = user.name;
};

//create a new user
User.create = (newUser, result) => {
  const attributes = "userID, name";
  const values = "\"" + newUser.userID + "\",\"" + newUser.name + "\"";

      sql.query("INSERT INTO User(" + attributes + ") VALUES(" + values + ")", (err,res) => {
          if(err){
              console.log("error: ", err);
              result(err, null);
              return;
          }
          console.log("created user: ", {...newUser });
          result(null, {...newUser});
      });
};

User.findById = (userID, result) => {

    sql.query('SELECT * FROM User WHERE userID=\"'+ userID + '\"', (err, res) => {
        if(err){
            console.log("error from db: ", err);
            result(err, null);
            return;
        }

        if(res.length) {
            console.log("found user: ", res[0]); // print user
            result(null, res);       //provide json
            return;
        }

        //not found User with the userID
        result({ kind: "not_found in db" }, null);
    });
};

User.updateById = (userID, user, result) => { // will deal with achievements here
    sql.query(
        "UPDATE User SET name = \"" + user.name + "\" WHERE userID = \"" + userID + "\"",
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0) {
                //not found user with userId
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("updated user: ", {...user });
            result(null, { ...user });
        }
    );
};

User.remove = (userID, result) => {
    sql.query(
        "DELETE FROM User WHERE userID = \"" + userID + "\"",
        (err, res) => {
            if(err) {
                console.log("error: ", err);
                result(null, err);
                return;
            }

            if(res.affectedRows == 0){
                //not found user with userID
                result({ kind: "not_found" }, null);
                return;
            }

            console.log("deleted user with userID: ", userID);
            result(null, res);
        }
    );
};

User.getAll = (result) => {
    sql.query("SELECT * FROM User", (err, res) => {
        if(err) {
            console.log("error: ", err);
            result(null, err);
            return;
        }

        console.log("users: ", res);
        result(null, res);
    });
};

module.exports = User;
