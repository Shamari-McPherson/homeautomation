# #!/usr/bin/python3

# #################################################################################################################################################
# #                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
# #################################################################################################################################################

# class DB:

#     def __init__(self, Config):

#         from math import floor
#         from os import getcwd
#         from os.path import join
#         from json import loads, dumps, dump
#         from datetime import timedelta, datetime, timezone 
#         from pymongo import MongoClient , errors, ReturnDocument
#         from urllib import parse
#         from urllib.request import  urlopen 
#         from bson.objectid import ObjectId  
       
      
#         self.Config                         = Config
#         self.getcwd                         = getcwd
#         self.join                           = join 
#         self.floor                          = floor 
#         self.loads                          = loads
#         self.dumps                          = dumps
#         self.dump                           = dump  
#         self.datetime                       = datetime
#         self.ObjectId                       = ObjectId 
#         self.server                         = Config.DB_SERVER
#         self.port                           = Config.DB_PORT
#         self.username                       = parse.quote_plus(Config.DB_USERNAME)
#         self.password                       = parse.quote_plus(Config.DB_PASSWORD)
#         self.remoteMongo                    = MongoClient
#         self.ReturnDocument                 = ReturnDocument
#         self.PyMongoError                   = errors.PyMongoError
#         self.BulkWriteError                 = errors.BulkWriteError  
#         self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


#     def __del__(self):
#         # Delete class instance to free resources
#         pass
 


#     ####################
#     # LAB 4 FUNCTIONS  #
#     ####################
    
#     # 1. CREATE FUNCTION TO INSERT DATA IN TO THE RADAR COLLECTION
#     def insert_data(self, data):
#         '''Inserts data into the radar collection'''
#         try:
#             # Connect to MongoDB
#             remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password, self.server, self.port), tls=self.tls)
#             result = remotedb.ELET2415.radar.insert_one(data)
#         except Exception as e:
#             msg = str(e)
#             if "duplicate" not in msg:
#                 print("insert_data error ",msg)
#             return False
#         else:                  
#             return True

#     # 2. CREATE FUNCTION TO RETRIEVE ALL DOCUMENTS FROM RADAR COLLECT BETWEEN SPECIFIED DATE RANGE. MUST RETURN A LIST OF DOCUMENTS
#     def getAllInRange(self, start, end):
#         '''RETURNS A LIST OF OBJECTS. THAT FALLS WITHIN THE START AND END DATE RANGE'''
#         try:
#             start = int(start)
#             end = int(end)
#             remotedb    = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
#             result      = remotedb.ELET2415.radar.find({'timestamp': {'$gte': start, '$lte': end}},{'_id': 0}).sort('timestamp', 1)
#         except Exception as e:
#             msg = str(e)
#             print("getAllInRange error ",msg)            
#         else:                  
#             return result

#     # 3. CREATE A FUNCTION TO COMPUTE THE ARITHMETIC AVERAGE ON THE 'reserve' FEILED/VARIABLE, USING ALL DOCUMENTS FOUND BETWEEN SPECIFIED START AND END TIMESTAMPS. RETURNS A LIST WITH A SINGLE OBJECT INSIDE
#     def calculate_avg_reserve(self, start_timestamp, end_timestamp):
#         try:
#             remotedb    = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
#             result = list(remotedb.ELET2415.radar.aggregate([{ "$match": { "timestamp": { "$gte": start_timestamp, "$lte": end_timestamp } } },{ "$group": { "_id": 0, "average": { "$avg": "$reserve" } } },{ "$project": { "_id": 0 } }]))
            
#             if result:
#                 return result[0]['average']
#             else:
#                 return 0
#         except Exception as e:
#             print(f"calculate_avg_reserve error: {str(e)}")
#             return 0

#     # 4. CREATE A FUNCTION THAT INSERT/UPDATE A SINGLE DOCUMENT IN THE 'code' COLLECTION WITH THE PROVIDED PASSCODE
#     def update_passcode(self, passcode):
#         '''ADD A NEW STORAGE LOCATION TO COLLECTION'''
#         try:
#             remotedb    = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
#             # Upsert: insert if not exists, update if exists
#             result      = find_one_and_update({}, { '$set': {'code': passcode}}, upsert=True, projection={'_id': False})
#             # remotedb.ELET2415.code.insert_one({"type": "passcode"},{"$set": {"passcode": passcode}}, projection={'_id': False},upsert=True)
            
#         except Exception as e:
#              msg = str(e)
#              if "duplicate" not in msg:
#                print("update_passcode error ",msg)
#                return False
#              else:                  
#                 return True
   
        
#     # 5. CREATE A FUNCTION THAT RETURNS A COUNT, OF THE NUMBER OF DOCUMENTS FOUND IN THE 'code' COLLECTION WHERE THE 'code' FEILD EQUALS TO THE PROVIDED PASSCODE.
#     def validate_passcode(self, passcode):
#         '''VALIDATE THE PASSCODE'''
#         try:
#             remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password, self.server, self.port), tls=self.tls)

#             # Check if a document with the given passcode exists
#             count = remotedb.ELET2415.code.count_documents({"code": passcode})

#             if count > 0:
#                 return True
#             else:
#                 return False
#         except Exception as e:
#             msg = str(e)
#             if "duplicate" not in msg:
#                 print("validate_passcode error ", msg)
#                 return False
#             else:
#                 return True

#     def updateData(self, data):
#             ''' Add a timestamp to the received data and publish the modified data to a subscribed topic '''
#             try:
                
#                 #Insert data into the radar collection
#                 remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
#                 result = remotedb.ELET2415.radar.insert_one(data)
#             except Exception as e:
#                 msg = str(e)
#                 print("updateData Error: ", msg)
#             else:
#                 return result
#     # REMEMBER, THE SCHEMA FOR THE SINGLE DOCUMENT IN THE 'code' COLLECTION IS {"type":"passcode","code":"0070"}

# def main():
#     from config import Config
#     from time import time, ctime, sleep
#     from math import floor
#     from datetime import datetime, timedelta
#     one = DB(Config)
 
 
#     start = time() 
#     end = time()
#     print(f"completed in: {end - start} seconds")
    
# if __name__ == '__main__':
#     main()

 #!/usr/bin/python3


#################################################################################################################################################
#                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
#################################################################################################################################################


class DB:

    def __init__(self,Config):

        from math import floor
        from os import getcwd
        from os.path import join
        from json import loads, dumps, dump
        from datetime import timedelta, datetime, timezone 
        from pymongo import MongoClient , errors, ReturnDocument
        from urllib import parse
        from urllib.request import  urlopen 
        from bson.objectid import ObjectId  
       
      
        self.Config                         = Config
        self.getcwd                         = getcwd
        self.join                           = join 
        self.floor                      	= floor 
        self.loads                      	= loads
        self.dumps                      	= dumps
        self.dump                       	= dump  
        self.datetime                       = datetime
        self.ObjectId                       = ObjectId 
        self.server			                = Config.DB_SERVER
        self.port			                = Config.DB_PORT
        self.username                   	= parse.quote_plus(Config.DB_USERNAME)
        self.password                   	= parse.quote_plus(Config.DB_PASSWORD)
        self.remoteMongo                	= MongoClient
        self.ReturnDocument                 = ReturnDocument
        self.PyMongoError               	= errors.PyMongoError
        self.BulkWriteError             	= errors.BulkWriteError  
        self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


    def __del__(self):
            # Delete class instance to free resources
            pass
 

    
    ####################
    # LAB 4 FUNCTIONS  #
    ####################
    
    # 1. CREATE FUNCTION TO INSERT DATA IN TO THE RADAR COLLECTION
    def insertData(self, data):
        ''' Insert data into the radar collection '''
        try:
            # Insert data into the radar collection
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = remotedb.ELET2415.radar.insert_one(data)
        except Exception as e:
            msg = str(e)
            print("insertData Error: ", msg)    
        else:
            return result

 
    # 2. CREATE FUNCTION TO RETRIEVE ALL DOCUMENTS FROM RADAR COLLECT BETWEEN SPECIFIED DATE RANGE. MUST RETURN A LIST OF DOCUMENTS

    def setPwd(self, passcode):
        ''' Update the document in the 'code' collection with the new passcode '''
        passcode = str(passcode)
        try:
            # Update the document in the 'code' collection with the new passcode
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            item = remotedb.ELET2415.code.find_one_and_update({}, { '$set': {'code': passcode}}, upsert=True, projection={'_id': False})
        except Exception as e:
            msg = str(e)
            print("setPwd Error: ", msg)
        else:
            return item 

    # 7. CREATE A FUNCTION THAT CHECKS THE PASSCODE AGAINST THE 'code' COLLECTION
    def checkPwd(self, passcode):
        ''' Validate passcode against the 'code' collection '''
        try:
            # Validate passcode against the 'code' collection
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            count = remotedb.ELET2415.code.count_documents({'code': passcode})
        except Exception as e:
            msg = str(e)
            print("checkPwd Error: ", msg)
        else:
            return count


   
    def updateData(self, data):
        ''' Add a timestamp to the received data and publish the modified data to a subscribed topic '''
        try:
            
            #Insert data into the radar collection
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = remotedb.ELET2415.radar.insert_one(data)
        except Exception as e:
            msg = str(e)
            print("updateData Error: ", msg)
        else:
            return result
            
    def retrieveData(self, start, end):
        ''' Retrieve all documents from the radar collection between the specified date range '''
        start = int(start)
        end = int(end)
        try:
            # Retrieve all documents from the radar collection between the specified date range
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result = remotedb.ELET2415.radar.find({'timestamp': {'$gte': start, '$lte': end}})
        except Exception as e:
            msg = str(e)
            print("retrieveData Error: ", msg)
        else:
            return result

    # 3. CREATE A FUNCTION TO COMPUTE THE ARITHMETIC AVERAGE ON THE 'reserve' FEILED/VARIABLE, USING ALL DOCUMENTS FOUND BETWEEN SPECIFIED START AND END TIMESTAMPS. RETURNS A LIST WITH A SINGLE OBJECT INSIDE
    def calculateAverage(self, start, end):
        ''' Compute the arithmetic average on the 'reserve' field/variable using all documents found between specified start and end timestamps '''
        start = int(start)
        end = int(end)
        try:
            # Retrieve all documents from the radar collection between the specified date range
            remotedb = self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            documents = list(remotedb.ELET2415.radar.aggregate([
                        {
                            '$match': {
                                'timestamp': {
                                    '$gte': start, 
                                    '$lte': end
                                }
                            }
                        }, {
                            '$group': {
                                '_id': None, 
                                'average': {
                                    '$avg': '$reserve'
                                }
                            }
                        }, {
                            '$project': {
                                '_id': 0, 
                                'average': 1
                            }
                        }
                    ]))
        except Exception as e:
            msg = str(e)
            print("compute_average Error: ", msg)
        else:
            return documents
   



def main():
    from config import Config
    from time import time, ctime, sleep
    from math import floor
    from datetime import datetime, timedelta
    one = DB(Config)
 
 
    start = time() 
    end = time()
    print(f"completed in: {end - start} seconds")
    
if __name__ == '__main__':
    main()


    