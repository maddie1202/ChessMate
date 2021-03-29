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