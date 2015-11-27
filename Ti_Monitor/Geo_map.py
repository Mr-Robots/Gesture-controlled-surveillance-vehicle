import json
import urllib
import sys

try:
    try:
        latitudes = sys.argv[1]
    except:
        sys.exit()
    api_url = "https://maps.googleapis.com/maps/api/geocode/json?latlng="+sys.argv[1]+"&key=AIzaSyDYBU3i9hN5SjDqO2n-zP36lpV3aFURvaQ"
    location_data = json.loads(urllib.urlopen(api_url).read())
    print json.dumps(location_data['results'][0]['formatted_address'])
except:
    print "Error : can not find location!"
