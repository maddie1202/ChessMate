/* Database Initialization Script: */
CREATE DATABASE ChessMateDB;
USE ChessMateDB;

/* User(userID, name) */
CREATE TABLE User(
    userID varchar(400) NOT NULL,
    name varchar(40) NOT NULL,
    PRIMARY KEY(userID)
);

/* Game(gameID, startDateTime, difficulty) */
CREATE TABLE Game(
    gameID int NOT NULL AUTO_INCREMENT,
    startDateTime datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    difficulty int,
    PRIMARY KEY (gameID)
);

/* Board(boardID, placements, gameID, sequenceNum) */
CREATE TABLE Board(
    boardID int NOT NULL AUTO_INCREMENT,
    placements varchar(13000) NOT NULL,
    gameID int NOT NULL,
    sequenceNumber int NOT NULL,
    PRIMARY KEY (boardID),
    FOREIGN KEY (gameID) REFERENCES Game (gameID) ON DELETE CASCADE
);

/* Results(userID, gameID, result) */
CREATE TABLE Results(
	userID varchar(400),
	gameID int,
	result int,
	PRIMARY KEY(userID, gameID),
	FOREIGN KEY (userID) REFERENCES User (userID) ON DELETE CASCADE,
	FOREIGN KEY (gameID) REFERENCES Game (gameID) ON DELETE CASCADE
);

/* GoalsLookup(goalID, gameCount, gameDifficulty) */
CREATE TABLE GoalsLookup(
	goalID int NOT NULL AUTO_INCREMENT,
	gameCount int NOT NULL,
	gameDifficulty int,
	PRIMARY KEY(goalID)
);

/* Achievements(userID, goalID) */
CREATE TABLE Achievements(
    userID varchar(400),
    goalID int,
    difficulty int,
    reqCount int,
    realCount int,
    PRIMARY KEY (userID, goalID),
    FOREIGN KEY (userID) REFERENCES User (userID) ON DELETE CASCADE,
    FOREIGN KEY (goalID) REFERENCES GoalsLookup (goalID) ON DELETE CASCADE
);

/* some starter values to insert into db to test apis */
INSERT INTO User(userID, name) VALUES('testUser123','testUser');

INSERT INTO Game(gameID, difficulty) VALUES(1, 1);
INSERT INTO Game(gameID, difficulty) VALUES(2, 1);

INSERT INTO Board(boardID, placements, gameID, sequenceNumber)
    VALUES(1, '1 2 3 4 5', 1, 1);
INSERT INTO Board(boardID, placements, gameID, sequenceNumber)
    VALUES(2, '5 6 7 8', 1, 2);

INSERT INTO Board(boardID, placements, gameID, sequenceNumber)
    VALUES(3, '11 22 33 44 55', 2, 1);
INSERT INTO Board(boardID, placements, gameID, sequenceNumber)
    VALUES(4, '55 66 77 88', 2, 2);


INSERT INTO Results(userID, gameID, result) VALUES('testUser123',1,0);
INSERT INTO Results(userID, gameID, result) VALUES('testUser123',2,1);

INSERT INTO GoalsLookup(goalID, gameCount, gameDifficulty)
    VALUES(1, 1, 1);
INSERT INTO GoalsLookup(goalID, gameCount, gameDifficulty)
    VALUES(2, 2, 1);
    
/*********************************************************/
