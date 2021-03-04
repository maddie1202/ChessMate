/* Database Initialization Script: */
CREATE DATABASE ChessMateDB;
USE ChessMateDB;

/* User(userID, name) */
CREATE TABLE User(
    userID int,
    name varchar(40),
    PRIMARY KEY(userID)
);

/* Game(gameID, startDateTime, difficulty) */
CREATE TABLE Game(
    gameID int,
    startDateTime datetime NOT NULL,
    difficulty int,
    PRIMARY KEY (gameID)
);

/* Board(boardID, placements, gameID, sequenceNum) */
CREATE TABLE Board(
    boardID int,
    placements varchar(13000),
    gameID int,
    sequenceNumber int,
    PRIMARY KEY (boardID),
    FOREIGN KEY (gameID) REFERENCES Game ON DELETE CASCADE
);

/* Results(userID, gameID, result) */
CREATE TABLE Results(
	userID int,
	gameID int,
	result int,
	PRIMARY KEY(userID, gameID),
	FOREIGN KEY (userID) REFERENCES User ON DELETE CASCADE,
	FOREIGN KEY (gameID) REFERENCES Game ON DELETE CASCADE
);

/* GoalsLookup(goalID, gameCount, gameDifficulty) */
CREATE TABLE GoalsLookup(
	goalID int,
	gameCount int,
	gameDifficulty int,
	PRIMARY KEY(goalID)
);

/* Achievements(userID, goalID) */
CREATE TABLE Achievements(
    userID int,
    goalID int,
    PRIMARY KEY (userID, goalID),
    FOREIGN KEY (userID) REFERENCES User ON DELETE CASCADE,
    FOREIGN KEY (goalID) REFERENCES GoalsLookup ON DELETE CASCADE
);
