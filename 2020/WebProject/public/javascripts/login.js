
var vueinst = new Vue(
	{
		el:'#vuemain',
		data: {
			successful_login: true
		}
	}
);

//CHECK LOGIN: Checks the credentials entered, logs the user in or sends an error message.
function check_login()
{

	//Creates the information that the user typed in
	var alleged_user = {email: "", password: ""};

	alleged_user.email = document.getElementById("email").value;
	alleged_user.password = document.getElementById("pass").value;


	//Ajax check password
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax request successful");

			//If account creation succesful, redirect to the dashboard.
			window.location.replace("dashboard.html");
			//else change vueinst.successful_login to false.
		}
		else if (this.status == 500 || this.status == 401)
		{
			vueinst.successful_login = false;
			console.log("Error logging in.");
		}
	};
	xhttp.open("POST", "/users/login", true);
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({alleged_user: alleged_user}));
}