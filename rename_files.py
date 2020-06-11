import os

filename = 0
def print_dirs_recursively(root_dir):
    root_dir = os.path.abspath(root_dir)
    global filename
    for item in os.listdir(root_dir):
        item_full_path = os.path.join(root_dir, item)
        if os.path.isdir(item_full_path):
            print_dirs_recursively(item_full_path)
        else:
            item_new_path = os.path.join(root_dir, str(filename))
            filename += 1
            os.rename(item_full_path, item_new_path + ".json")

            print("RENAMED: " + item_full_path + " ----> " + item_new_path + ".json")

