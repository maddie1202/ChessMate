/*
 Here we define constructor for "User" object here
 and use the db connection to write CRUD functions:

 - create a new user
 - find a user by userId
 - update a user by userId
 - remove a user by userId
*/

const sql = require("./db.js");

/*
 * userId: a user's unique id
 * games: a user's past games ()
 */
const User = function(user) {
  this.user_id = user.user_id;
}

User.create = (userId, result) => {

}

User.findById = (userId, result) => {

}

User.updateById = (userId, result) => {

}

User.remove = (userId, result) => {

}
