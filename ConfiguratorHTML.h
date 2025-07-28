#ifndef CONFIGURATOR_HTML_H
#define CONFIGURATOR_HTML_H

namespace ConfiguratorHTML {
static const char* AP_HEAD = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Setup</title>
</head>
<body>)rawliteral";

static const char* AP_SSID_ERROR = R"rawliteral(
<p style="color: red;">SSID is required.</p>
)rawliteral";

static const char* AP_SAVED_CONFIG = R"rawliteral(
<p>SSID: $ssid</p>
<p>Configuration saved. Restarting after 5 seconds...</p>
)rawliteral";

static const char* AP_FORM = R"rawliteral(
<form method="POST" action="/" style="display: flex; flex-direction: column; gap: 1em; max-width: 30em;">
	<input type="hidden" name="setup" value="setup">

	<div style="display: flex; gap: 1em;">
		<label for="ssid">SSID:</label>
		<input type="text" id="ssid" name="ssid" required style="flex: 1;">
	</div>

	<div style="display: flex; gap: 1em;">
		<select id="ssid_list"
			onchange="document.getElementById('ssid').value=this.value;"
			style="flex: 1;">
			<option value="" selected disabled></option>
		</select>
		<input type="button" value="Scan SSID" onclick="scanSSID()">
	</div>

	<div style="display: flex; gap: 1em;">
		<label for="wifiPassword">WIFI Password:</label>
		<input type="password" id="wifiPassword" name="wifiPassword" style="flex: 1;">
	</div>

	<div style="display: flex; justify-content: flex-end;">
		<input type="submit" value="Save">
	</div>
</form>

<script>
function scanSSID() {
    var scanBtn = document.querySelector('input[value="Scan SSID"]');
    scanBtn.disabled = true;
    fetch('/scan')
        .then(response => response.json())
        .then(data => {
            var ssidList = document.getElementById('ssid_list');
            ssidList.innerHTML = '';
            data.forEach(function(ssid) {
                var option = document.createElement('option');
                option.value = ssid;
                option.textContent = ssid;
                ssidList.appendChild(option);
            });
            if (data.length > 0) {
                document.getElementById('ssid').value = data[0]; // Set the first SSID as default
            }
        })
        .catch(error => {
            alert('Failed to scan networks');
        })
        .finally(() => {
            scanBtn.disabled = false;
        });
}
</script>
)rawliteral";

static const char* AP_FOOT= R"rawliteral(
</body>
</html>)rawliteral";
};
#endif