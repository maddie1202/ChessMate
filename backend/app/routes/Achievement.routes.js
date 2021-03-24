module.exports = app => {
  const Ach = require('../controllers/Achievement.controller.js');

  // Get all goals from goalsLookup
  app.get("/getgoals", Ach.findAllGoals);

  // Get everything from Achievements table
  app.get("/getgoalsstatus/:userID", Ach.findAllGoalsStatus);

  // Get all achievements from Achievements table
  app.get("/getgoalsdone/:userID", Ach.findAllCompleted);
}