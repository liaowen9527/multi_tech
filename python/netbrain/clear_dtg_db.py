from pymongo import MongoClient
import os


def load_tenants_byfsc(mongo_conn, fsc_id):
    db_ngsystem = mongo_conn.get_database("NGSystem")
    fsc_doc_list = db_ngsystem.FrontServerController.find({'uniqueName': fsc_id})

    tenants = []
    for fsc_doc in fsc_doc_list:
        tenant_obj_list = fsc_doc.get('tenants')
        for  tenant_obj in tenant_obj_list:
            tenants.append(tenant_obj.get('name'))
    
    return tenants

def drop_dtg_collections(mongo_conn, tenant_dbname):
    db_teannt = mongo_conn.get_database(tenant_dbname)
    collection_name_list = ['FSC_DtgRecvDataCounter', 'FSC_DataTaskGroup', 'FSC_DataTaskGroup_DispatchInfo']
    for collection in collection_name_list:
        db_teannt.drop_collection(collection)
        #db_teannt.collection.drop()

def remove_fs_dtg_dbfile():
    filepath = r"C:\netbrain_code\NG\CrossPlatform\package\data\DataTaskDB.db"
    if os.path.exists(filepath):
        #删除文件，可使用以下两种方法。
        os.remove(filepath)

def main():
    uri = "mongodb://10.10.0.30:27017/admin?authMechanism=SCRAM-SHA-1"
    fsc_id = 'liaowen_fsc'

    mongo_conn = MongoClient(uri)
    tenants = load_tenants_byfsc(mongo_conn, fsc_id)
    for tenant_name in tenants:
        drop_dtg_collections(mongo_conn, tenant_name)

    remove_fs_dtg_dbfile()
    
main()