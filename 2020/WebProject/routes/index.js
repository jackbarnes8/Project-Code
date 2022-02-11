var express = require('express');
var router = express.Router();

var sanitizeHtml = require('sanitize-html');    //Enables Sanitize HTML

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});


module.exports = router;





//
//	TASK ROUTES
//


	//GET INITIAL TASKS: Runs upon loading the dashboard and logging in.
router.post('/get-initial-tasks', function(req, res) {

		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		//old query: SELECT * FROM Tasks INNER JOIN Users ON Tasks.roster = Users.roster WHERE Tasks.roster = ?;
		var query = "SELECT * FROM Tasks WHERE Tasks.roster = ?;";
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


	//ADD TASK: Adds a task to the SQL Database
router.post('/add-task', function(req, res) {

  req.pool.getConnection( function(err, connection) {
    if(err) {
      console.log("Error connecting to SQL");
      res.sendStatus(500);
      return;
    }
    var query = "INSERT INTO Tasks (taskName, category, assignee, dueDate, priority, taskDescription, roster) VALUES (?, ?, ?, ?, ?, ?, ?);";
    connection.query(query, [sanitizeHtml(req.body.new_task.taskName), sanitizeHtml(req.body.new_task.category), sanitizeHtml(req.body.new_task.assignee), sanitizeHtml(req.body.new_task.dueDate), sanitizeHtml(req.body.new_task.priority), sanitizeHtml(req.body.new_task.taskDescription), sanitizeHtml(req.body.new_task.roster)], function(err, rows, fields) {
    connection.release();
      if(err)
      {
        console.log("Error inserting into SQL");
        console.log(err);
        res.sendStatus(500);
        return;
      }
    });
  });


  res.sendStatus(200);
});


	//COMPLETE TASK: This function removes a task from the servers task object, once it has been ticked as completed.
router.post('/remove-task', function(req, res) {
  console.log("Attempting removal of this task on the server side:");
  console.log(req.body.task.taskName);

req.pool.getConnection( function(err, connection) {
    if(err) {
      console.log("Error connecting to SQL");
      res.sendStatus(500);
      return;
    }
    var query = "DELETE FROM Tasks WHERE Tasks.taskName = ?;";
    connection.query(query, [req.body.task.taskName], function(err, rows, fields) {
    connection.release();
      if(err)
      {
        console.log("Error deleting from SQL");
        res.sendStatus(500);
        return;
      }
    });
  });

  res.sendStatus(200);
});


	//GET MY TASKS: Only returns tasks assigned to the current user.
router.post('/get-my-tasks', function(req, res) {

console.log("Got to here on the get my tasks route");

req.pool.getConnection( function(err, connection) {
    if(err) {
      console.log("Error connecting to SQL");
      res.sendStatus(500);
      return;
    }
    var query = "SELECT Tasks.taskName, Tasks.category, Tasks.assignee, Tasks.dueDate, Tasks.priority, Tasks.taskDescription, Tasks.roster FROM Tasks INNER JOIN Users ON Users.name = Tasks.assignee WHERE Users.email = ?;";
    connection.query(query, [req.body.userEmail], function(err, rows, fields) {
    connection.release();
      if(err)
      {
        console.log("Error deleting from SQL");
        res.sendStatus(500);
        return;
      }
      res.json(rows);

    });
  });

});








//
//	ROSTER ROUTES
//


	//ADD ROSTER: Adds a new roster to the SQL database. The manager is tracked by their email.
router.post('/add-roster', function(req, res) {
		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "INSERT INTO Rosters (name, manager) VALUES (?, ?);";
		connection.query(query, [sanitizeHtml(req.body.new_roster.rosterName), req.body.new_roster.manager], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			res.end();
		});
	});

});



	//GET ROSTER: Expects a users emails, returns the roster from SQL that that user has previously joined.
router.post('/get-roster', function(req, res) {
	var text = req.body.userEmail;
	console.log("Logging the input: " + text);

		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		var query = "SELECT Rosters.name, Rosters.manager, Rosters.id FROM Rosters INNER JOIN Users ON Rosters.name = Users.roster WHERE Users.email = ?;";
		connection.query(query, [req.body.userEmail], function(err, rows, fields) {
			connection.release();
			if(err) {
				//console.log(err);
				res.sendStatus(500);
				return;
			}
			console.log("Logging the sql request");
			console.log(rows[0]);

			res.json(rows[0]);
		});
	});

});



	//REQUEST ROSTER BY ID: Expects an ID, returns the roster that matches that ID (and the manager)
router.post('/request-roster-by-id', function(req, res) {

	console.log("Joining roster with this ID: " + req.body.joiningRosterID);

		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		//Update the users roster attribute in the SQL
		var query = "SELECT Rosters.name, Rosters.manager FROM Rosters WHERE Rosters.id = ?";
		connection.query(query, [sanitizeHtml(req.body.joiningRosterID)], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			res.json(rows);


		});
	});
});


	//JOIN ROSTER
router.post('/join-roster', function(req, res) {

		req.pool.getConnection(function(err, connection) {
		if(err) {
			res.sendStatus(500);
			return;
		}

		//Update the users roster attribute in the SQL
		var query = "UPDATE Users SET roster = ? WHERE Users.email = ?;";
		connection.query(query, [sanitizeHtml(req.body.joiningRoster), req.body.userEmail], function(err, rows, fields) {
			connection.release();
			if(err) {
				res.sendStatus(500);
				return;
			}
			res.end();

		});
	});
});