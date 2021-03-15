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
 * games: a user's past games ()
 */
const User = function(user) {
  this.user_id = user.user_id;
  this.name = user.name;
}

User.create = (newUser, result) => {
  // add a new user to User db
  // initialize goals in Goal db
  sql.query("INSERT INTO User SET ?", newUser, (err,res) => {
      if(err) {
          console.log("error: ", err);
          result(err, null);
          return;
      }
      console.log("created user: ", {id: newUser.user_id, name: newUser.name });
      result(null, {id: newUser.user_id, name: newUser.name }));
  });
}

User.findById = (userId, result) => {

}

User.updateById = (userId, result) => { // will deal with achievements here

}

User.remove = (userId, result) => {

}

module.exports = User;
