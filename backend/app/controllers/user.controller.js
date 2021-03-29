const User = require("../models/user.model.js");
const Game = require("../models/Game.model.js");

exports.create = (req,res) => {
    //validate request
    if (!req.body.userID) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
    }

      // Create a User
      const newUser = new User({
        userID: req.body.userID,
        name: req.body.name
      });

      // Save User in the database
      User.create(newUser, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while creating the User."
          });
        else res.send(data);
      });
};

exports.findOne = (req,res) => {
    const userID = req.params.userID;
    User.findById(userID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found User with userID ${userID}.`
                });
              } else {
                res.status(500).send({
                  message: "Error retrieving User with userID " + userID
                });
              }
        } else res.send(data);
    });
};

exports.update = (req,res) => {
      // Validate Request
      if (!req.body) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
      }

      const userID = req.body.userID;
      // Create a User
      const user = new User({
          userID: req.body.userID,
          name : req.body.name
      });

      User.updateById(userID, user, (err, data) => {
        if (err) {
            if (err.kind === "not_found") {
              res.status(404).send({
                message: `Not found User with userID ${userID}.`
              });
            } else {
              res.status(500).send({
                message: "Error updating User with userID " + userID
              });
            }
          } else res.send(data);
      });
};

exports.delete = (req,res) => {

    Game.removeAll(userID, (err, data) => {
        if(err){
            const res_message = "Some error while deleting associated games, see more : " + err;
            res.send({message: res_messgae });
        }
    });

    const userID = req.params.userID;

    User.remove(userID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found User with userID ${userID}.`
                });
              } else {
                res.status(500).send({
                  message: "Could not delete User with userID " + userID
                });
              }
        } else res.send({ message: `User was deleted successfully!` });
    });
};

exports.findAll = (req, res) => {
    User.getAll((err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving users."
          });
        else res.send(data);
    });
};