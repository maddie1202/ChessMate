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
  //this.games = user.games; // collection of game ids - could be its own thing
  //this.doneAchievements = user.doneAchievements; // initialize to []
  //this.currAchievements = user.currAchievements; // in progress/not started
}

User.create = (userId, result) => {
  // add a new user to User db
  // initialize goals in Goal db
}

User.findById = (userId, result) => {

}

User.updateById = (userId, result) => { // will deal with achievements here

}

User.remove = (userId, result) => {

}

module.exports = User;
