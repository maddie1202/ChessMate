/*
create a user
Post method
url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeuser
body = {
    "userID" : "userID string",
    "name" : "some user name"
}

This will also initialize an achievements table 
/////////////////////////////////////////

start a game
POST method
url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makegame
body = {
    "difficulty" : number
}

This will create a game with current timestamp and provided difficulty, and
return a json response with gameID

response = {
    "gameID" : gameID
}
////////////////////////////////////////

post a board snapshot
POST method
url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard
Remember to keep track of seqnum for the gameID
body = {
    "placements" : "string that conatins the placement snapshot",
    "gameID" : gameID,
    "sequenceNum" : seqnum
}

response = {
    "id" : boardID,
    "placements" : "what you input in POST method",
    "gameID" : gameID,
    "sequenceNum" : seqnum
}
//////////////////////////////////////

post a game result
POST method
url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult

body = {
    "userID" : "userID string",
    "gameID" : gameID,
    "result" : null/0/1
}
////////////////////////////////////

get the status of all achievements for the user
GET method

Remember to input userID as params in url (see below in url)

url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgoalsstatus/userID

response = [
    {
        "userID": "userID",
        "goalID": 1,
        "difficulty": 1,
        "reqCount": 1,
        "realCount": 2
    },
    {
        "userID": "userID",
        "goalID": 2,
        "difficulty": 1,
        "reqCount": 2,
        "realCount": 2
    }
]
////////////////////////////////////

get all achievements ACQUIRED by the user
GET method

Remember to input userID as params in url

url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgoalsdone/userID

response = [
    {
        "userID": "userID",
        "goalID": 1,
        "difficulty": 1,
        "reqCount": 1,
        "realCount": 2
    },
    {
        "userID": "userID",
        "goalID": 2,
        "difficulty": 1,
        "reqCount": 2,
        "realCount": 2
    }
]
//////////////////////////////////////////
get the games for the user (sample flow of app)

step 1: get all gameIDs for that user
    GET method (remember to put userID in url)
    url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getallgames/userID
    response = [
    {
        "userID": "userID string",
        "gameID": some_gameID,
        "result": 0
    },
    {
        "userID": "userID string",
        "gameID": some_other_gameID,
        "result": 1
    },
    {
        "userID": "userID string",
        "gameID": another_gameID,
        "result": null
    }
]

step 2: display these games in the user screen
for every game in response above
    if you want to display start time and difficulty: 
        GET method (remember to put gameID in url = res[i].gameID)
        url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgamedetails/gameID

    if you want to get the last snapshot
        GET method (remember to put gameID in url)
        url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgame/gameID

        this will return reponse with all boards for this user
        now display the board with largest sequence number
/////////////////////////////////////////

get a game with all boards 
GET method

Remember to put gameID in url

url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getgame/gameID

response = [
    {
        "boardID": some_boardID,
        "placements": "1 2 3 4 5",
        "gameID": gameID,
        "sequenceNumber": 1
    },
    {
        "boardID": some_other_boardID,
        "placements": "5 6 7 8",
        "gameID": gameID,
        "sequenceNumber": 2
    }
]
//////////////////////////////////////////

CLEAN UP FOR THE USER


//////////////////////////////////////////
delete a user
DELETE method

Remember to put userID in url 

url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/deleteuser/userID

response = {
    "message": "User was deleted successfully!"
}

This deletes all games in Games, 
all results in Results, 
all boards for games, and 
Achievements table
/////////////////////////////////////////////

OPTIONAL APIS we might need

//////////////////////////////////////////////
delete all games for that user
DELETE method

Remember to put userID in url

url = http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/deleteallgames/userID

response = {
    "message": " x games deleted!"
}
////////////////////////////////////////////


*/