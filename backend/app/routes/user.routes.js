
module.exports = app => {
  const User = require('../controllers/user.controller.js');

  // Create a new user with userID
  app.post("/makeuser/:userID", User.create);

  // Retrieve a single user with userID
  app.get("/getuser/:userID", User.findOne);

  // Update a user with userID
  app.put("/updateuser/:userID", User.update);

  // Delete a user by userID
  app.delete("/deleteuser/:userID", User.delete);

  //list all users in the db
  app.get("/getallusers", User.findAll);

}
