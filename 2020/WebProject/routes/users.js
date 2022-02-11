var express = require('express');
var router = express.Router();

var sanitizeHtml = require('sanitize-html');    //Enables Sanitize HTML
var argon2 = require('argon2'); //Enables argon2

/* GET users listing. */
router.get('/', function(req, res, next) {
  res.send('respond with a resource');
});



  //ARGON2 SIGNUP
router.post('/signup', async function(req, res, next) {

  console.log("Testing signup");
  var phash = null;
  try
  {
    phash = await argon2.hash(req.body.new_user.password);
    req.pool.getConnection(function(err, connection) {
		if(err) {
		  console.log("Error connecting to SQL");
			res.sendStatus(500);
			return;
		}

		var query = "INSERT INTO Users (name, email, password) VALUES (?, ?, ?);";
		connection.query(query, [req.body.new_user.name, req.body.new_user.email, phash], function(err, rows, fields) {
			connection.release();
			if(err) {
			  console.log("Error inserting into SQL, see the error below.");
			  console.log(err);
				res.sendStatus(500);
				return;
			}
    console.log("Successful signup.");
    req.session.user = req.body.new_user.email;
    //res.redirect(301, '/dashboard');

    res.end();

		});
	});
  }
  catch (err)
  {
    console.log("Error using argon2");
    res.sendStatus(500);
    return;
  }

});


  //ARGON2 LOGIN
router.post('/login', async function(req, res, next) {


	console.log("Testing sanitising login");

  var password = sanitizeHtml(req.body.alleged_user.password);
  var username = sanitizeHtml(req.body.alleged_user.email);

  console.log("Password is " + password);
  console.log("email is " + username);

  //grab the password from the database
  try {
  req.pool.getConnection(function(err, connection) {
    if(err) {
      console.log("Error connecting to SQL");
			res.sendStatus(500);
			return;
    }
    var query = "SELECT * FROM Users WHERE Users.email = ?;"
    connection.query(query, [username], async function(err, rows, fields) {
      connection.release()
      if(err) {
			  console.log("Error getting credentials: wrong email?");
			  //console.log(err);
				res.sendStatus(500);
				return;
			}

			if (await argon2.verify(rows[0].password, password)) {
      // password match

      req.session.user = username;
      res.sendStatus(200);

    }
    else
    {
      // password did not match
      console.log("Password did not match.");
      res.sendStatus(401);
      return;
    }

    });
  });
  }
  catch (err)
  {
    // internal failure
    console.log("Internal failure.");
    res.sendStatus(500);
    return;
  }


});


//LOGOUT: Logs the user out and deletes the session variable.
router.get('/logout', function(req, res) {

  delete req.session.user;
  res.sendStatus(200);

});



//
//  USER SETS AND GETS
//


  //GET SESSION: Returns the session token for the signed up user
router.get('/get-session', function(req, res) {
  res.send(req.session.user);
});



  //GET USERNAME: Expects an email address and returns the username
router.post('/get-username', function(req, res) {
  		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "SELECT Users.name FROM Users WHERE Users.email = ?";
		connection.query(query, [req.body.userEmail], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			console.log(rows);
			res.json(rows[0]);
		});
	});

});


  //GET ROSTER USERS: Returns all the users of the given roster, and also their availabilities. For the manager setting tasks. Should populate an object in dashboard.js.
router.post('/get-roster-users', function(req, res) {

		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "SELECT Users.name, Users.availability FROM Users WHERE Users.roster = ?";
		connection.query(query, [req.body.roster], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			console.log(rows);
			res.json(rows);
		});
	});



});


  //UPDATE AVAILABILITY: Updates the users availability in the SQL database.
router.post('/update-availability', function(req, res) {

  var userEmail = req.session.user;

  	req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "UPDATE Users SET availability = ? WHERE Users.email = ?";
		connection.query(query, [req.body.availability_string, userEmail], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			res.end();
		});
	});

});


  //GET AVAILABILITY: Retrieves the users availability as a string
router.get('/get-availability', function(req, res) {

  var userEmail = req.session.user;

  	req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "SELECT Users.availability FROM Users WHERE Users.email = ?";
		connection.query(query, [userEmail], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			res.json(rows);
		});
	});

});




module.exports = router;
