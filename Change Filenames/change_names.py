import os

def rename_files(folder_path, order_path):
    with open(order_path, 'r') as order_file:
        new_names = order_file.read().splitlines()

    if len(new_names) == 0:
        print("No new names found in the order file.")
        return

    file_names = sorted(os.listdir(folder_path))
    if len(new_names) != len(file_names):
        print("Number of new names does not match the number of files.")
        return

    for i in range(len(file_names)):
        old_path = os.path.join(folder_path, file_names[i])
        new_name = f"{i + 1} {new_names[i]}.txt" # Change the file extension here as needed
        new_path = os.path.join(folder_path, new_name)

        if os.path.exists(new_path):
            print(f"Warning: File '{new_name}' already exists. Skipping.")
        else:
            os.rename(old_path, new_path)
            print(f"Renamed '{file_names[i]}' to '{new_name}'")

if __name__ == "__main__":
    folder_path = "FILE_PATH_HERE"  # Change this to the path of the folder containing the files
    order_path = "new_file_name.txt"  # Change this to the path of the order text file

    rename_files(folder_path, order_path)