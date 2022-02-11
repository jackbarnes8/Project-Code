
var vueinst = new Vue( 		//create a new vue instance
	{
		el:'#vuemain', //"apply to this id please"

		data: {			//variables of our vue instance
		task_object: [],			//object to hold tasks. FOR THE CURRENT TASKS OF THIS ROSTER
		user_object: [], 			//Contains user objects FOR THE CURRENT USERS OF THIS ROSTER

		has_tasks: false, 			//if false, show "you have no tasks" instead of the table containing tasks
		creating_new_task: false, 	//true if creating a task, false otherwise. Intended to switch from the task table view to the creating new task view.

		roster_users: [],			//Users that belong to this roster
		list_groups: [], 			//list of groups
		part_of_roster: false, 		//boolean for if the user is in a roster or not
		joining_roster: false,		//Show the joining roster div
		creating_roster: false,		//if currently creating a roster
		creating_group: false, 		//if creating a group
		current_roster: "", 		//the current roster
		is_manager: false,			//Is the user the manager of this roster?
		roster_id: "",				//id of the joined roster

		current_user: "",			//the current user. Set to the users email, retrieved from the session token.
		username: "",				//Username for displaying in the top right
		selected_task_object: {taskName:'No task selected', category:'', assignee:'', dueDate:'', taskDescription:''},
		new_task_object: {taskName:'', category:'', assignee:'', dueDate:'', priority:'', description:'', roster: ''},
		},




		methods: {
			show_task_info: function (input_task)							//Updates the right side bar with information on the task
			{
				this.selected_task_object.taskName = input_task.taskName;
				this.selected_task_object.category = input_task.category;
				this.selected_task_object.assignee = input_task.assignee;
				this.selected_task_object.dueDate = input_task.dueDate;
				this.selected_task_object.taskDescription = input_task.taskDescription;
			},
			change_creating_new_tasks: function ()							//Changes the view from the table to the creating new task view
			{
				if (this.creating_new_task === false)
				{
					this.creating_new_task = true;
				}
				else
				{
					this.creating_new_task = false;
				}
			},
			change_creating_roster: function ()
			{
				if(this.creating_roster === false)
				{
					this.creating_roster = true;
				}
				else
				{
					this.creating_roster = false;
				}
			},
			change_joining_roster: function ()
			{
				if(this.joining_roster === false)
				{
					this.joining_roster = true;
				}
				else
				{
					this.joining_roster = false;
				}
			},
			change_creating_group: function ()
			{
				if(this.creating_group === false)
				{
					this.creating_group = true;
				}
				else
				{
					this.creating_group = false;
				}
			},
			//COMPLETE TASK: Marks a task as complete and removes it from the SQL database
			mark_complete: function (input_task)
			{
				//Ajax to remove the task from this task object and also remove it from the SQL database.
				var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
						if (this.readyState == 4 && this.status == 200) {
							console.log("Remove task Ajax worked");
						}
					};
					xhttp.open("POST", "/remove-task", true);
					xhttp.setRequestHeader("Content-type", "application/json");
					xhttp.send(JSON.stringify({task: input_task}));

				//Update the object here (remove the completed task)
				for(let i = 0; i < this.task_object.length; i++)
				{
					if(input_task.taskName === this.task_object[i].taskName)
					{
						this.task_object.splice(i,1);
						if(this.task_object.length === 0)
						{
							this.has_tasks = false;
						}

						break;
					}
				}
			}
		}
	}
);


//
//	USER FUNCTIONS
//

//GET SESSION: Checks the current session and sets the current_user in the vue instance. The first function to run upon loading the dashboard.
function get_session()
{
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					console.log("The session token is:");
					console.log(this.response);

					vueinst.current_user = this.response;
					console.log("Therefore the current user is " + vueinst.current_user);
                }
            };
			xhttp.open("GET", "/users/get-session", false); 	//NOTE: YOU CHANGED THIS TO FALSE. FOR THE INITAL GENERATION I THINK THIS IS NECESSARY
            xhttp.send();
}

//GET USERNAME: Gets the username (not email) for displaying in the top right.
function get_username()
{
	var userEmail = vueinst.current_user;

	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					console.log("Testing getting username...");
					var username = (JSON.parse(this.response));
					vueinst.username = username.name;
					console.log("Your username is " + vueinst.username);
                }
            };
			xhttp.open("POST", "/users/get-username", false);
			xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send(JSON.stringify({userEmail: userEmail}));
}

//GET ROSTER USERS: Sets the vue instance's roster_users object to contain all users of this roster. For grouping tasks and assigning tasks.
function get_roster_users()
{
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					console.log("Getting all the users of this roster...");
					var temp_roster_users = JSON.parse(this.response);
					console.table(temp_roster_users);

					if(temp_roster_users.length !== 0)
					{
						for(let i = 0; i < temp_roster_users.length; i++)
						{
							vueinst.roster_users.push(temp_roster_users[i]);
						}

						console.table(vueinst.roster_users);
					}
                }
            };
			xhttp.open("POST", "/users/get-roster-users", false); 	//NOTE: YOU CHANGED THIS TO FALSE. FOR THE INITAL GENERATION I THINK THIS IS NECESSARY
			xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send(JSON.stringify({roster: vueinst.current_roster}));
}

//LOGOUT: Logs the user out
function logout()
{
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					window.location.replace("index.html");
                }
            };
			xhttp.open("GET", "/users/logout", true);
            xhttp.send();
}



//
//	TASK FUNCTIONS
//


//CREATE NEW TASK: Creates a new task and sends it to the SQL database
function create_new_task()
{
	//Update the task object in the vue instance
	var new_task = { taskName:"", category:"", assignee:"", dueDate:"", priority:"", taskDescription:"", roster: "" };


	new_task.taskName = document.getElementById("form_tname").value;
	new_task.category = document.getElementById("form_category").value;
	new_task.assignee = document.getElementById("user-select").value;
	new_task.dueDate = document.getElementById("form_duedate").value;
	new_task.priority = document.getElementById("form_priority").value;
	new_task.taskDescription = document.getElementById("form_description").value;
	new_task.roster = vueinst.current_roster;

	vueinst.task_object.push(new_task);
	vueinst.has_tasks = true; 			//update has_tasks to flag that there is a task now
	vueinst.creating_new_task = false;	//return back to the table view

	//AJAX to update the object on the server side
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					console.log("Ajax worked");
                }
            };
			xhttp.open("POST", "/add-task", true);
			xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send(JSON.stringify({new_task: new_task}));	//may need to send each of the parameters over individually
}



//GET INITIAL TASKS: Sends the current roster over to the server, and returns all the tasks in that roster from the SQL database
function get_initial_tasks()
{
	var roster = vueinst.current_roster;

	//Retrieve tasks from the server side and update them here.
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {

			var temp_all_tasks = JSON.parse(this.response); 	//Store the object sent over by the route
			console.log("These are the tasks:");
			console.table(temp_all_tasks);

			vueinst.task_object.length = 0; 			//clear the existing task object

			if(temp_all_tasks.length !== 0)
			{
				vueinst.has_tasks = true; 						//There are tasks, so we set this to true.

				for(let i = 0; i < temp_all_tasks.length; i++) 	 	//Add the tasks sent from the response to our vueinst task object
				{
					vueinst.task_object.push(temp_all_tasks[i]);
				}
			}
		}
	};
	xhttp.open("POST", "/get-initial-tasks", false);
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({roster: roster}));
}

//GET MY TASKS
function get_my_tasks()
{
	var userEmail = vueinst.current_user;

	//Get the tasks
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					var temp_my_tasks = JSON.parse(this.response);
					console.table(temp_my_tasks);
					vueinst.task_object.length = 0;			//frees up the task array

					if(temp_my_tasks.length !== 0)
					{
						vueinst.has_tasks = true;

						for(let i = 0; i < temp_my_tasks.length; i++)
						{
							vueinst.task_object.push(temp_my_tasks[i]);
						}
					}
					else
					{
						vueinst.has_tasks = false;
					}

                }
            };
			xhttp.open("POST", "/get-my-tasks", false);
			xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send(JSON.stringify({userEmail: userEmail}));
}




//
//	ROSTER FUNCTIONS
//


//CREATE ROSTER: Creates a new roster and adds it to the SQL Database
function create_roster()
{
	var new_roster = {rosterName: "", manager: ""};

	//Declare the new roster values
	new_roster.rosterName = document.getElementById("form_rname").value;
	new_roster.manager = vueinst.current_user;



	//Ajax to add the roster to SQL
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax worked for creating roster");
		}
	};
	xhttp.open("POST", "/add-roster", true);
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({new_roster: new_roster})); 		//send the new roster object to the server

	//Ajax to set the user as part of that roster
	var userEmail = vueinst.current_user;
	var joiningRoster = new_roster.rosterName;

	var xhttp2 = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax worked for setting the roster");
			//Flag the booleans
			vueinst.current_roster = new_roster.rosterName; 		//update the current roster for the user
			vueinst.part_of_roster = true;
			vueinst.is_manager = true;			//This user is the manager of this roster, since they created it
			vueinst.creating_roster = false;	//They are no longer creating a roster so they'll be returned to the normal view
		}
	};
	xhttp2.open("POST", "/join-roster", true);
	xhttp2.setRequestHeader("Content-type", "application/json");
	xhttp2.send(JSON.stringify({joiningRoster: joiningRoster, userEmail: userEmail}));

}


//GET ROSTER: Retrieves the current roster of the user by sending their email to check against the SQL database which roster to return. Also checks if the current user is the manager of that roster. INITIAL.
function get_roster()
{
	var userEmail = vueinst.current_user;

	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax worked for getting roster");
			if(this.response !== undefined)
			{
				var retrievedRoster = JSON.parse(this.response);
				console.log("Here is the roster object returned from SQL");
				console.log(retrievedRoster);
				console.log(retrievedRoster.manager);

				//perhaps a conditional to check if it isn't null
				vueinst.current_roster = retrievedRoster.name; //sets the current roster
				vueinst.roster_id = retrievedRoster.id;
				vueinst.part_of_roster = true;
				if(retrievedRoster.manager === userEmail)
				{
				vueinst.is_manager = true;
				}
			}
		}
	};
	xhttp.open("POST", "/get-roster", false);						//NOTE: YOU CHANGED THIS TO FALSE. FOR THE INITAL GENERATION I THINK THIS IS NECESSARY
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({userEmail: userEmail}));


}

//JOIN ROSTER: Retrieves a roster via its ID, then sets the roster for the user once they join it. Also checks for if a manager is joining the roster.
function join_roster()
{
	var joiningRosterID = document.getElementById("form_rid").value;
	vueinst.roster_id = joiningRosterID;
	var userEmail = vueinst.current_user;

	//Ajax to get name of that roster via its ID
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax worked for requesting roster by ID");
			joiningRoster = JSON.parse(this.response); //sets as the current roster

			vueinst.current_roster = joiningRoster[0].name;
			vueinst.part_of_roster = true;
			vueinst.joining_roster = false;

			if(vueinst.current_user === joiningRoster[0].manager)
			{
				vueinst.is_manager = true;
			}
			get_initial_tasks();
			get_roster_users();
		}
	};
	xhttp.open("POST", "/request-roster-by-id", false); 							//NOT ASYNC
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({userEmail: userEmail, joiningRosterID: joiningRosterID}));


	//Ajax to join that roster by sending the name and users email
	var xhttp2 = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
	if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax worked for join roster");

		}
	};
	xhttp2.open("POST", "/join-roster", false);									//NOT ASYNC
	xhttp2.setRequestHeader("Content-type", "application/json");
	xhttp2.send(JSON.stringify({userEmail: userEmail, joiningRoster: joiningRoster[0].name}));


}