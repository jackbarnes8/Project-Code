var vueinst = new Vue (
	{
		el:'#vuemain',

		data: {
			current_setting: 'Profile', 		//starts off as profile
			current_user: '',
			availabilities_updated: false
		},

		methods: {
			switch_to_availability: function ()
			{
				this.current_setting = 'Availability';
				get_availability();
			},
			switch_to_profile: function ()
			{
				this.current_setting = 'Profile';
				this.availabilities_updated = false;
			}
		}
	}
);

	//GET SESSION:
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
			xhttp.open("GET", "/users/get-session", false);
            xhttp.send();
}

	//UPDATE AVAILABILITIES: Sends the days that the user has checked to SQL.
function update_availability()
{
	var availability_string = "";
	if(document.getElementById("monday").checked)
	{
		availability_string += "Mon ";
	}
	if(document.getElementById("tuesday").checked)
	{
		availability_string += "Tue ";
	}
	if(document.getElementById("wednesday").checked)
	{
		availability_string += "Wed ";
	}
	if(document.getElementById("thursday").checked)
	{
		availability_string += "Thu ";
	}
	if(document.getElementById("friday").checked)
	{
		availability_string += "Fri ";
	}

	//AJAX to update the users availability
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					console.log("Ajax worked");
					vueinst.availabilities_updated = true;
                }
            };
			xhttp.open("POST", "users/update-availability", true);
			xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send(JSON.stringify({availability_string: availability_string}));

}


	//GET AVAILABILITIES: Upon loading the availability section, the checkboxes will need to be updated with the previous availabilities that the user declared
function get_availability()
{
	//AJAX to retrieve the availabilities and update the checkboxes
	var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
					var availabilities = this.response;
					console.log(availabilities);
					//Check for occurences of days of the week in the response
					if(availabilities.indexOf("Mon") !== -1)
					{
						document.getElementById("monday").checked = true;
					}
					if(availabilities.indexOf("Tue") !== -1)
					{
						document.getElementById("tuesday").checked = true;
					}
					if(availabilities.indexOf("Wed") !== -1)
					{
						document.getElementById("wednesday").checked = true;
					}
					if(availabilities.indexOf("Thu") !== -1)
					{
						document.getElementById("thursday").checked = true;
					}
					if(availabilities.indexOf("Fri") !== -1)
					{
						document.getElementById("friday").checked = true;
					}

                }
            };
			xhttp.open("GET", "users/get-availability", true);
            xhttp.send();
}

