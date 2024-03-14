$.ajaxSetup({ cache: false });
  $(document).ready(function update() {
      $.getJSON('/stuff', function (dat) {
          $("#temp").text("Temp: " + dat['tempf'] + String.fromCharCode(176) + "F");
          $("#hum").text("Humidity: " + dat['humidity'] + "%");
	  $("#maxtemp").text("Highest Temp: " + dat['maxf'] + String.fromCharCode(176) + "F");
	  $("#mintemp").text("Lowest Temp: " + dat['minf'] + String.fromCharCode(176) + "F");
	  $("#uptimed").text(dat['uptime']);
      });
      var interval = setTimeout(update, 1000);
  });
  
