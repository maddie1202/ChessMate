module.exports = app => {

    const game = require("../controllers/Game.controller.js");

    //Retrieve a single Game with gameId
    app.get("/getgamedetails/:gameID", game.findOne);

    //Retrieve all games for user with userId
    app.get("/getallgames/:userID", game.findAll);

}