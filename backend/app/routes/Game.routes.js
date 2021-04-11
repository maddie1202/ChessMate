module.exports = app => {

    const game = require("../controllers/Game.controller.js");

    //Retrieve a single Game with gameId
    app.get("/getgamedetails/:gameID", game.findOne);

    //Retrieve all games for user with userId
    app.get("/getallgames/:userID", game.findAll);

    //Retrieve latest game
    app.get("/getlatestgame/:userID", game.findLatest);

    //create a game with gameID and game details
    app.post("/makegame", game.create);

    //update a game with gameID and game details
    app.put("/updategame", game.update);

    //delete a game with gameID
    app.delete("/deletegamedetails/:gameID", game.delete);

    //delete all games using userID
    app.delete("/deleteallgames/:userID", game.deleteAll);

    //post the game result
    app.post("/postresult", game.createResult);

    //update the game result
    app.put("/updateresult", game.updateResult);

}