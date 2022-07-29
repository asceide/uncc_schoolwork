import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class Router {

    //Constructor
    public Router() {
        System.out.println("Constructor");
    }


    //Implemented By Roman Styrku:
    //This Method creates an array of subnet masks from the routing table.
    public static int[] getSubnet(Map<String, String> routingTable) {
        int[] subnet = new int[routingTable.size() - 1]; // initializes array for subnets
        String[] temp; //Initializes temp array to split ip and subnet into.
        int x = 0; //Temperary variable to use as iterator.
        for (String key : routingTable.keySet()) { //For loop to Iterate through routing table keys.
            temp = key.split("\\/"); //Splits the ip and the subnet in the key
            if (key != "Default") { // Tests if the key is default or not
                subnet[x] = Integer.parseInt(temp[1]); // Adds the subnet value to the array.
                x++; //Iterates to next element in array.
            }
        }
        return subnet; // Returns Subnet Array
    }


    //Implemented By Patrick Alarcon. Refactored by Roman Styrku:
    //This Method Takes an IP Address and Converts it to Binary Form
    public static String toBinary(String ipAddress) {
        StringBuilder binary = new StringBuilder();//StringBuild to hold the binary form of the ip address
        String[] ipToBinary = ipAddress.split("\\.");/* A string array that holds the IPv4 Octets using the
        . as a delimiter
        */
        for (int i = 0; i < ipToBinary.length; i++) {//loop that converts the IPv4 Address to binary
            int temp = Integer.parseInt(ipToBinary[i]);//parses the IPv4 octet to an Integer
            if (temp > 0 && temp <= 255) {//checks to see if the number is between 0 and 255
                binary.append(Integer.toBinaryString(temp));//converts the octet to binary
            }
            if (i < ipToBinary.length) { //If statement to test if i is the last octet.
                binary.append("."); //If not the last octet adds the "." between octets.
            }
        }
        binary.deleteCharAt(binary.length() - 1);//deletes the final character
        return (binary.toString());//returns the binary as a string
    }


    //Implemented By Patrick Alarcon, Refactored by Roman Styrku:
    /*This Method Takes a Binary IP Address and a Subnet Mask and Performs the AND Operation. Returns the Final
        Network IP in Binary Form
     */
    public static String getNetworkAddress(String binary, int mask) {
        String[] binaryAddress = binary.split("\\."); // Splits Address into Array
        String maskAdd = ""; //New string to hold binary of mask
        for (int i = 0; i < mask; i++) { //For loop to populate binary of the mask
            maskAdd += "1"; //Adds 1s for the first "mask" numbers
        }
        while (maskAdd.length() != 32) { //While loop for the rest of the binary string.
            maskAdd += "0"; // adds 0's to the rest of the string.
        }
        String[] maskAddress = new String[4]; //New array to divide mask
        maskAddress[0] = maskAdd.substring(0, 8); //Sent first octet to array
        maskAddress[1] = maskAdd.substring(8, 16); // Send second octet to array
        maskAddress[2] = maskAdd.substring(16, 24); // Send third octet to array
        maskAddress[3] = maskAdd.substring(24, 32); // Send fourth octet to array

        String[] temp = new String[4]; //Creates a temp array to create full octets for IP Address
        for (int i = 0; i < 4; i++) { //For loop to populate temp from binaryAddress
            temp[i] = binaryAddress[i];
        }

        for (int i = 0; i < 4; i++) { //While loop to iterate through temp
            while (temp[i].length() < 8) { //For loop to add 0's to the left hand side till length is 8
                temp[i] = "0" + temp[i];
            }
        }

        String netAdd = ""; //New String to hold Binary Final Network IP
        for (int i = 0; i < 4; i++) { //For Loop to Iterate through the Octets
            for (int j = 0; j < 8; j++) { //For Loop to Iterate through each value in the Octet
                if (temp[i].charAt(j) == maskAddress[i].charAt(j)) { //Compares values to perform and operation
                    netAdd += temp[i].charAt(j); //If equal adds value to netАdd
                } else { // Else Statement to add automatic 0 if not equal
                    netAdd += "0";
                }
            }
            if (i < 3) { //If statement to add decimal if its not the end of the string.
                netAdd += ".";
            }
        }
        return netAdd; //Returns Final Network Address

    }


    //Implemented by Rushi Patel. Refactored by Roman Styrku:
    //This Method Takes a Binary IP Address and converts it back into Decimal Form
    public static String converter(String binaryNumber) { //Method to convert binary to decimal
        String[] binAdd = binaryNumber.split("\\."); //Splits up the number into an array.
        int[] binAddress = new int[4]; //Array to hold int values
        for (int i = 0; i < 4; i++) { //For loop to parse the string numbers to int by base 2.
            binAddress[i] = (Integer.parseInt(binAdd[i], 2));
        }

        String netAdd = ""; //String variable to hold final IP
        for (int i = 0; i < 4; i++) { //Loop to add decimal points
            netAdd += binAddress[i];
            if (i < 3) {
                netAdd += ".";
            }
        }
        return netAdd; //Return IP

    }


    //Implemented by Patrick Alarcon:
    //This method takes the Network IP and routes it to the right hop from the Table.
    public static String ipNetRoute(String netIP, Map routeTable) {
        String iP = "Default"; //By default it set to hop to the "Default" mask.
        for (Object key : routeTable.keySet()) { // For Loop to Test Each Mask if it Fits.
            if (key.equals(netIP)) { //If statement to break out of the loop if a destination is found.
                iP = netIP;
                break;
            }
        }
        return routeTable.get(iP).toString(); //Returns the final Hop from the Table,
    }


    //Combined by Roman Styrku:
    //Final Method combining Everything
    public static String routingMethod(String ipAddress, Map<String, String> table) {
        int[] subnetList = getSubnet(table); //Creates array of possible subnets
        String route = ""; //Initializes the final String Route
        for (int i = 0; i < subnetList.length; i++) { //For loop to test all possible masks
            String binAdd = toBinary(ipAddress); //Performs the conversion to binary
            String netBin = getNetworkAddress(binAdd, subnetList[i]); //Performs AND operation with mask IP
            String binToDec = converter(netBin); //Performs the conversion to decimal
            String netIP = binToDec + "/" + subnetList[i]; //Adds subnet to the IP
            route = ipNetRoute(netIP, table); //Routes the IP Using the Table.

            if (route != table.get("Default")) { //Breaks loop if route is not "Default"
                break;
            }
        }

        return route; //Returns the final Route
    }
}

