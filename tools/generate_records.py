"""
This script generates sample sighting records for rubber-duckies project.
Sighting records is sourced from emails, a web form, and mobile app.

Raw data is contained in three file formats txt, csv, and json. 
Both json and csv file formats store structured data.

JSON data structure: 
{
 "latitude": FLOAT,
 "longitude": FLOAT,
 "date": STRING <year-month-day>,
 "description": STRING
}

CSV data structure: 
STRING Date <year-month-day>, FLOAT latitude, FLOAT longitude , STRING description

Data stored in .txt format reprsents gathered from email is unstructed and provides no content garantees.

txt record example:
June 10 2024: This morning I saw a bright yellow rubber duck was spotted
cruising leisurely on the Thames River in London (51 30' 26''N, 7' 39''W).
It had a tiny blue scarf around its neck.

"""

import os
import json
import csv
import argparse
import random

OUTPUT_DIR = "test-data"    # specifies name of output directory for generated sighting records
NUM_RECORDS = 10           # specifies number of sighting records to generate for each file format

def generate_json_record(num_records: int) -> None:
    for i in range(num_records):
        # generate random sighting record
        record = {
            "latitude": round(random.uniform(-90, 90), 6),
            "longitude": round(random.uniform(-180, 180), 6),
            "date": f"{random.randint(2020, 2024)}-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}",
            "description": f"Sample sighting description {i+1}"
        }

        # write each record to a separate JSON file
        with open(os.path.join(OUTPUT_DIR, f"sighting_{i+1:02d}.json"), "w") as json_file:
            json.dump(record, json_file, indent=4)

    print(f"Generated {num_records} JSON records in {OUTPUT_DIR}/sightings.json")

def generate_csv_record(num_records: int) -> None:
    with open(os.path.join(OUTPUT_DIR, "sightings.csv"), "w", newline="") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["Date", "Latitude", "Longitude", "Description"])  # write header

        for i in range(num_records):
            date = f"{random.randint(2020, 2024)}-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}"
            latitude = round(random.uniform(-90, 90), 6)
            longitude = round(random.uniform(-180, 180), 6)
            description = f"Sample sighting description {i+1}"
            writer.writerow([date, latitude, longitude, description])

    print(f"Generated {num_records} CSV records in {OUTPUT_DIR}/sightings.csv")

def generate_txt_record(num_records: int) -> None:
    for i in range(num_records):
        date = f"{random.randint(2020, 2024)}-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}"
        latitude = round(random.uniform(-90, 90), 6)
        longitude = round(random.uniform(-180, 180), 6)

        with open(os.path.join(OUTPUT_DIR, f"sighting_{i+1:02d}.txt"), "a") as txt_file:
            txt_file.write(f"{date}: A bright yellow rubber duck was spotted at coordinates ({latitude}, {longitude}).\n")

    print(f"Generated {num_records} txt records in {OUTPUT_DIR}/sightings.txt")

def generate_sample_record(num_records: int) -> None:
    generate_json_record(num_records)
    generate_csv_record(num_records)
    generate_txt_record(num_records)

def parse_command_line_arguments():
    global OUTPUT_DIR, NUM_RECORDS

    # set up command line argument parser
    parser = argparse.ArgumentParser(description="Generate sample sighting data for rubber-duckies project.")
    parser.add_argument("-n", "--num-records", type=int, default=NUM_RECORDS, help="Number of sample records to generate.")
    parser.add_argument("-o", "--output-dir", type=str, default=OUTPUT_DIR, help="Directory to save generated sample data.")
    
    # parse command line arguments
    args = parser.parse_args() 
    if args.num_records < 1:
        print("Number of records must be a positive integer")
    else:
        NUM_RECORDS = args.num_records

    if args.output_dir: 
        OUTPUT_DIR = args.output_dir

def make_output_directory():
    # create directory output if it does not already exist 
    try: 
        os.makedirs(OUTPUT_DIR)
        print(f"Created output: {OUTPUT_DIR}")
    except FileExistsError:
        print(f"Output directory already exists: {OUTPUT_DIR}")
    except PermissionError: 
        print(f"Permission denied: Unable to create output directory {OUTPUT_DIR}")
    except Exception as e:
        print(f"An error occurred while creating output directory: {e}")

def main():
    parse_command_line_arguments()
    make_output_directory()
    generate_sample_record(NUM_RECORDS)

if __name__ == "__main__":
    main()