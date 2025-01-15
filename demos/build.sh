#!/bin/bash

# Paths and Directories
MinGW_Path="/c/msys64/usr/bin"
export PATH="$MinGW_Path:$PATH"

# Global variables
INCLUDE_DIRS=("../core")
FILE_NAME=""
OUTPUT=""
BUILD_DLL=false  # Flag for building shared DLL

# Generate include flags
INCLUDE_FLAGS=""
for dir in $(find "${INCLUDE_DIRS[@]}" -type d); do
    INCLUDE_FLAGS+="-I$dir "
done

# Parse program input
function parse_program_input() {
    local input="$1"
    
    if [[ "$input" == *"-dll" ]]; then
        BUILD_DLL=true
        program_name="${input%-dll}"  # Strip '-dll' from the input
    else
        BUILD_DLL=false
        program_name="$input"
    fi

    if [[ -f "$(pwd)/code/$program_name.cpp" ]]; then
        if [[ "$BUILD_DLL" == true ]]; then
            OUTPUT="$(pwd)/dynamic_libs/$program_name"
        else
            OUTPUT="$(pwd)/executables/$program_name"
        fi

        FILE_NAME="$(pwd)/code/$program_name.cpp"
    elif [[ -f "$(pwd)/code/$program_name.hpp" ]]; then
        if [[ "$BUILD_DLL" == true ]]; then
            OUTPUT="$(pwd)/dynamic_libs/$program_name"
        else
            OUTPUT="$(pwd)/executables/$program_name"
        fi

        FILE_NAME="$(pwd)/code/$program_name.hpp"
    else
        echo "Invalid source file: $program_name"
        exit 1
    fi
}

# Main loop
while true; do
    read -p "Enter the program name (e.g., Test or Test-dll) or -1 to exit: " program_input
    if [[ "$program_input" == "-1" ]]; then
		read -n 1 -s -r -p "Press any key to exit..."
        echo "Exiting..."
        exit 0
    fi

    parse_program_input "$program_input"

	if $BUILD_DLL; then
		# Build shared DLL
		DLL_OUTPUT="${OUTPUT}.dll"
		DLL_IMPORT_LIB="${OUTPUT}.a"
		g++ $INCLUDE_FLAGS "$FILE_NAME" -shared -o "$DLL_OUTPUT" -Wl,--out-implib,"${DLL_IMPORT_LIB}"

		# Check compilation success
		if [ $? -ne 0 ]; then
			echo "DLL creation failed!"
			read -n 1 -s -r -p "Press any key to exit..."
			exit 1
		fi

		echo "DLL creation successful! Output: $DLL_OUTPUT and Import Library: $DLL_IMPORT_LIB"
		
		# Blank space
		echo -e "\n"
    else
		echo -e "its an exe\n"
        # Build executable
        g++ $INCLUDE_FLAGS "$FILE_NAME" -o "$OUTPUT"

        # Check compilation success
        if [ $? -ne 0 ]; then
            echo "Build failed!"
            read -n 1 -s -r -p "Press any key to continue..."
			
			# Blank space
			echo -e "\n"
        else
			echo "Build successful! Output: $OUTPUT"

			# Determine OS and set executable name
			OS_TYPE=$(uname)
			EXECUTABLE_NAME="$OUTPUT"
			if [[ "$OS_TYPE" == "Windows"* ]]; then
				EXECUTABLE_NAME="$OUTPUT.exe"
			fi

			# Run the executable
			echo -e "Starting the executable...\n"
			"$EXECUTABLE_NAME"

			# Blank space
			echo -e "\n"
		fi
    fi

    # Ask if the user wants to restart
    read -p "Do you want to build and run another program? (y/n): " restart_choice
    if [[ "$restart_choice" != "y" ]]; then
        echo "Exiting..."
        exit 0
    fi
done
