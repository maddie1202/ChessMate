

const Ach = require("../models/Achievement.model.js");

exports.findAllGoals = (req, res) => {

    Ach.getAllGoals((err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving goals."
          });
        else res.send(data);
    });
};

exports.findAllGoalsStatus = (req, res) => {

    const userID = req.params.userID;

    Ach.getAllGoalsStatus(userID, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving status of all goals for this user."
          });
        else res.send(data);
    });
};

exports.findAllCompleted = (req, res) => {

    const userID = req.params.userID;

    Ach.getAllCompleted(userID, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving all achieved goals for this user."
          });
        else res.send(data);
    });
};
