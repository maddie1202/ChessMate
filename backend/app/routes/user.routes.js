
module.exports = app => {
  const users = require('../controllers/user.controller.js');

  // Create a new user
  app.post("/users", users.create);

  // Retrieve a single Board with boardId
  app.get("/users/:userId", users.findOne);

  // Update a Board with boardId
  app.put("/users/:userId", users.update);

  // Delete a Board by boardId
  app.delete("/boards/:userId", users.delete);

}
