from pymongo import MongoClient
import datetime


def find_domain_differences(old_domain_entry, new_domain_entry, new_date):
    old_date = datetime.datetime.fromisoformat(old_domain_entry["last_change"])

    date_difference = old_date - new_date

    if date_difference.days >= 0:  # Ensure the datasets are in chronical order
        return None, None

    changes_text = []
    differences = {}
    for k, v in new_domain_entry.items():
        if k == "_id": continue

        print(k)
        if k not in old_domain_entry:  # if a new key doesnt exists in the old domain document
            changes_text.append({
                "date": str(new_date.date()),
                "change": f"Added {k} with value {v}"
            })
            differences[k] = v
        else:  # Compare two equal keys to see if their values differ
            old_v = old_domain_entry[k]
            if v != old_v:
                changes_text.append({
                    "date": str(new_date.date()),
                    "change": f"Updated {k} with value {old_v} --> {v}"
                })
                differences[k] = v
    return changes_text, differences


def fill_db_pymongo(new_domains_entry, new_date):
    client = MongoClient("localhost", 27017)

    db = client["my_db"]
    collection = db["domains"]

    for new_domain_entry in new_domains_entry:

        # Check existence of document
        document_filter = {'_id': new_domain_entry['_id']}
        count = collection.count_documents(document_filter)

        if count == 0:
            new_domain_entry["changes"] = []
            new_domain_entry["last_change"] = str(datetime.date.today())
            _id = collection.insert_one(new_domain_entry)
        elif count == 1:
            print("Looking for differences in document...")
            old_domain_entry = collection.find_one(document_filter)
            changes_text, differences = find_domain_differences(old_domain_entry, new_domain_entry, new_date)

            print(changes_text, differences)
            if differences and changes_text:
                differences["last_change"] = str(new_date.date()).strip()
                print("Handling differences in document...")
                update = {
                    '$push': {'changes': {
                        '$each': changes_text
                        }
                    },
                    '$set': differences,
                }
                collection.update_one(document_filter, update)
            else:
                print("No differences found. ")
        else:
            print(f"Error: More than 1 document with _id={_id}")


domains = [
    {
        "_id": "bilprisbasen.dk",
        "owner": "Julian Køster Larsen"
    },
    {
        "_id": "dalai-lama.dk",
        "owner": "Kim Larsen",
    },
]

if __name__ == "__main__":
    fill_db_pymongo(domains, datetime.datetime.fromisoformat("2020-03-08"))
