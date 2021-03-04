
module.exports = app => {
  const users = require('../controllers/user.controller.js');

  // Create a new user
  app.post("/users", users.create);

  // Retrieve a single user with userId
  app.get("/users/:userId", users.findOne);

  // Update a user with userId
  app.put("/users/:userId", users.update);

  // Delete a user by userId
  app.delete("/users/:userId", users.delete);

}
