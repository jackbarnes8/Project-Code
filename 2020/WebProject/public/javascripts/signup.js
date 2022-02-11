

var vueinst = new Vue(
	{
		el:'#vuemain',
		data: {
			successful_login: true
		}
	}
);

//CREATE ACCOUNT: Takes the credentials entered by the user and creates a new account.
function create_account()
{
	//Creates the new user object
	var new_user = {name:"", email:"", password:""};

	new_user.name = document.getElementById("name").value;
	new_user.email = document.getElementById("email").value;
	new_user.password = document.getElementById("pass").value;

	//Ajax to update it on the server side
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("Ajax request successful");

			window.location.replace("dashboard.html");
		}
		else
		{
			console.log("COULDN'T DO AJAX");
		}
	};
	xhttp.open("POST", "/users/signup", true);
	xhttp.setRequestHeader("Content-type", "application/json");
	xhttp.send(JSON.stringify({new_user: new_user}));


}
