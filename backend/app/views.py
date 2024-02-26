# """
# Flask Documentation:     https://flask.palletsprojects.com/
# Jinja2 Documentation:    https://jinja.palletsprojects.com/
# Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
# This file creates your application.
# """

# # from crypt import methods
# import site 

# from app import app, Config,  mongo, Mqtt
# from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
# from json import dumps, loads 
# from werkzeug.utils import secure_filename
# from datetime import datetime,timedelta, timezone
# from os import getcwd
# from os.path import join, exists
# from time import time, ctime
# from math import floor
 
 

# #####################################
# #   Routing for your application    #
# #####################################


# # 1. CREATE ROUTE FOR '/api/set/combination'
# @app.route('/api/set/combination', methods=['POST']) # POST REQUEST WITH FORM DATA
# def set_combination():   
#     '''Updates the passcode'''    
#     if request.method == "POST":
#         try:  
#             # EXTRACT FORM DATA
#             passcode = request.form.get('passcode')

#             # Check if passcode is a 4-digit integer
#             if passcode.isdigit() and len(passcode) == 4:
#                 # Update passcode
#                 sucess= mongo.update_passcode(passcode)
#                 data=list(item)
#                 if data:
#                     return jsonify({"status": "complete", "data": "complete"})
#                 else:
#                     return jsonify({"status": "failed", "data": "Passcode must be a 4-digit integer."})
#             else:
#                 return jsonify({"status": "failed", "data": "Passcode must be a 4-digit integer."})

#         except Exception as e:
#             print(f"set_combination error: {str(e)}")        
#     return jsonify({"status":"failed","data":str(e)})

    
# # 2. CREATE ROUTE FOR '/api/check/combination'
# @app.route('/api/check/combination', methods=['POST'])
# def check_combination():
#     '''Validates the passcode'''
#     if request.method == "POST":
#         try:
#             # Extract passcode from the form data
#             passcode = request.form.get('passcode')
#             # Validate the passcode against the database
#             result = validate_passcode(passcode)
#             if result:
#                 return jsonify({"status": "complete", "data": "complete"})
#             else:
#                 return jsonify({"status": "failed", "data": "Invalid passcode"})
#         except Exception as e:
#             print(f"check_combination error: {str(e)}")
#             return jsonify({"status": "failed", "data": str(e)})
#     return jsonify({"status": "failed", "data": "Invalid request method"})


# # 3. CREATE ROUTE FOR '/api/update' might need o change
# @app.route('/api/update', methods=['POST'])
# def update_data():
#     if request.method == "POST" and request.get_json()  :
#         try:
#             jsonDoc= request.get_json()
#             # Update the document in the 'code' collection with the new passcode
#             timestamp = datetime.now().timestamp()
#             timestamp = floor(timestamp)
#             jsonDoc['timestamp'] = timestamp
#             Mqtt.publish("620152241",json.dumps(jsonDoc))
#             Mqtt.publish("620152241_pub",json.dumps(jsonDoc))
#             Mqtt.publish("620152241_sub",json.dumps(jsonDoc))
#             print(f"MQTT: {jsonDoc}")

#             item = mongo.updateData(jsonDoc)
#             if item:
#                 return jsonify({"status": "complete", "data": "complete"})
#         except Exception as e:
#             msg = str(e)
#             print(f"update_data Error: {msg}")
#         return jsonify({"status": "failed", "data": "failed"})

# # 4. CREATE ROUTE FOR '/api/reserve/<start>/<end>'
# @app.route('/api/reserve/<start>/<end>', methods=['GET']) 
# def get_all(start,end):   
#     start = int(start)
#     end = int(end)
#     '''RETURNS ALL THE DATA FROM THE DATABASE THAT EXIST IN BETWEEN THE START AND END TIMESTAMPS'''
#     # print(f"Start Date: {start}")
#     # print(f"End Date: {end}")
#     # print(type(start))
#     # print(type(end))
#     if request.method == "GET":
#         '''Add your code here to complete this route'''
#         try:
#             item = mongo.getAllInRange(start,end)
#             data = list(item)
#             if data:
#                 return jsonify({"status":"found","data": data})
            
#         except Exception as e:
#             print(f"get_all error: f{str(e)}") 

#     # FILE DATA NOT EXIST
#     return jsonify({"status":"not found","data":[]})

# # 5. CREATE ROUTE FOR '/api/avg/<start>/<end>'
# @app.route('/api/avg/<start>/<end>', methods=['GET'])
# def get_average_reserve(start, end):
#     start_timestamp = int(start)
#     end_timestamp = int(end)
    
#     if request.method == "GET":
#         try:
#             average = calculate_avg_reserve(start_timestamp, end_timestamp)
            
#             if average:
#                 return jsonify({"status": "found", "data": average})
#             else:
#                 return jsonify({"status": "failed", "data": 0})
#         except Exception as e:
#             print(f"get_average_reserve error: {str(e)}")
#             return jsonify({"status": "failed", "data": 0})

   






# @app.route('/api/file/get/<filename>', methods=['GET']) 
# def get_images(filename):   
#     '''Returns requested file from uploads folder'''
   
#     if request.method == "GET":
#         directory   = join( getcwd(), Config.UPLOADS_FOLDER) 
#         filePath    = join( getcwd(), Config.UPLOADS_FOLDER, filename) 

#         # RETURN FILE IF IT EXISTS IN FOLDER
#         if exists(filePath):        
#             return send_from_directory(directory, filename)
        
#         # FILE DOES NOT EXIST
#         return jsonify({"status":"file not found"}), 404


# @app.route('/api/file/upload',methods=["POST"])  
# def upload():
#     '''Saves a file to the uploads folder'''
    
#     if request.method == "POST": 
#         file     = request.files['file']
#         filename = secure_filename(file.filename)
#         file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
#         return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


# ###############################################################
# # The functions below should be applicable to all Flask apps. #
# ###############################################################


# @app.after_request
# def add_header(response):
#     """
#     Add headers to both force latest IE rendering engine or Chrome Frame,
#     and also tell the browser not to cache the rendered page. If we wanted
#     to we could change max-age to 600 seconds which would be 10 minutes.
#     """
#     response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
#     response.headers['Cache-Control'] = 'public, max-age=0'
#     return response

# @app.errorhandler(405)
# def page_not_found(error):
#     """Custom 404 page."""    
#     return jsonify({"status": 404}), 404


"""
Flask Documentation:     https://flask.palletsprojects.com/
Jinja2 Documentation:    https://jinja.palletsprojects.com/
Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
This file creates your application.
"""

# from crypt import methods
import site 
import json

from app import app, Config,  mongo, Mqtt
from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
from json import dumps, loads 
from werkzeug.utils import secure_filename
from datetime import datetime,timedelta, timezone
from os import getcwd
from os.path import join, exists
from time import time, ctime
from math import floor
from pymongo import ReturnDocument
 


#####################################
#   Routing for your application    #
#####################################

# 1. CREATE ROUTE FOR '/api/set/combination'
@app.route('/api/set/combination', methods=['POST'])
def set_pwd():
    # Retrieve the passcode from the 'code' collection in the databas
    passcode = request.json.get('code')

    print(f"passcode: {passcode}")

    if request.method == "POST" :
        try:
                # Update the document in the 'code' collection with the new passcode
                item= mongo.setPwd(passcode)
                if item:
                    return jsonify({"status":"complete","data":"complete"})
        except Exception as e:
            msg=str(e)  
            print(f"update_pwd Error: f{msg}")
        return jsonify({"status":"failed","data":"failed"})
  
# 2. CREATE ROUTE FOR '/api/check/combination'
@app.route('/api/check/combination', methods=['POST'])
def check_pwd():
# Retrieve the passcode from the 'code' collection in the database
    passcode = request.form.get('passcode')

    if request.method == "POST":
        try:
            # Validate passcode against the 'code' collection
            count = mongo.checkPwd(passcode)
            if count > 0:
                return jsonify({"status": "complete", "data": "complete"})
        except Exception as e:
            msg = str(e)
            print(f"check_combination Error: {msg}")
        return jsonify({"status": "failed", "data": "failed"})

# 3. CREATE ROUTE FOR '/api/update'        
@app.route('/api/update', methods=['POST'])
def update_data():
    if request.method == "POST" and request.get_json()  :
        try:
            jsonDoc= request.get_json()
            # Update the document in the 'code' collection with the new passcode
            timestamp = datetime.now().timestamp()
            timestamp = floor(timestamp)
            jsonDoc['timestamp'] = timestamp
            Mqtt.publish("620152241",json.dumps(jsonDoc))
            Mqtt.publish("620152241_pub",json.dumps(jsonDoc))
            Mqtt.publish("620152241_sub",json.dumps(jsonDoc))
            print(f"MQTT: {jsonDoc}")

            item = mongo.updateData(jsonDoc)
            if item:
                return jsonify({"status": "complete", "data": "complete"})
        except Exception as e:
            msg = str(e)
            print(f"update_data Error: {msg}")
        return jsonify({"status": "failed", "data": "failed"})

# 4. CREATE ROUTE FOR '/api/reserve/<start>/<end>'
@app.route('/api/reserve/<start>/<end>', methods=['GET'])
def retrieve_data(start, end):
    # Retrieve the passcode from the 'code' collection in the database
    start = int(start)
    end = int(end)
    if request.method == "GET":
        try:
            # Retrieve all documents from the radar collection between the specified date range
            result = mongo.retrieveData(start, end)
            if result:
                return jsonify({"status": "complete", "data": "complete"})
        except Exception as e:
            msg = str(e)
            print(f"reserve Error: {msg}")
        return jsonify({"status": "failed", "data": 0})

# 5. CREATE ROUTE FOR '/api/avg/<start>/<end>'
@app.route('/api/avg/<start>/<end>', methods=['GET'])
def calculate_average(start, end):
    # Retrieve the start and end values from the URL parameters
    start = int(start)
    end = int(end)
    print(f"start: {start}, end: {end}")
    print(type(start), type(end))

    if request.method == "GET":
        try:
            # Calculate the average of the data between the start and end values
            item = mongo.calculateAverage(start, end)
            data= list(item)
            print(f"calculate_average: {data}")
            if data:
            # Return the calculated average as a JSON response
                return jsonify({"status": "complete", "data": data})
        except Exception as e:
            msg = str(e)
            print(f"calculate_average Error: {msg}")
        return jsonify({"status": "failed", "data": "failed"})


@app.route('/api/file/get/<filename>', methods=['GET']) 
def get_images(filename):   
    '''Returns requested file from uploads folder'''
   
    if request.method == "GET":
        directory   = join( getcwd(), Config.UPLOADS_FOLDER) 
        filePath    = join( getcwd(), Config.UPLOADS_FOLDER, filename) 

        # RETURN FILE IF IT EXISTS IN FOLDER
        if exists(filePath):        
            return send_from_directory(directory, filename)
        
        # FILE DOES NOT EXIST
        return jsonify({"status":"file not found"}), 404


@app.route('/api/file/upload',methods=["POST"])  
def upload():
    '''Saves a file to the uploads folder'''
    
    if request.method == "POST": 
        file     = request.files['file']
        filename = secure_filename(file.filename)
        file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
        return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


###############################################################
# The functions below should be applicable to all Flask apps. #
###############################################################


@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also tell the browser not to cache the rendered page. If we wanted
    to we could change max-age to 600 seconds which would be 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.errorhandler(405)
def page_not_found(error):
    """Custom 404 page."""    
    return jsonify({"status": 404}), 404
