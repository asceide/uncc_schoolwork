//Graphic User Interface: Implemented by Roman Styrku

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

public class RouteMyIP {
    //Initializes each GUI Component
    private JTable RoutingTable;
    private JList list1;
    private JTextField textField1;
    private JButton button1;
    private JPanel GUIPanel;

    public RouteMyIP() {

        //Sets What happens when the Route Button is clicked
        button1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                //Initialized the Routing Table Data in the GUI
                Map<String, String> table = new HashMap<String, String>();
                table.put("135.46.56.0/22", "Interface 0");
                table.put("135.46.60.0/22", "Interface 1");
                table.put("192.53.40.0/23", "Router 1");
                table.put("Default", "Router 2");

                //Gets Users input in the Text Field
                String test = textField1.getText();

                //Displays final route based on user input and routing table.
                JOptionPane.showMessageDialog(null, test + " will route to: " + Router.routingMethod(test, table));
            }
        });

        //Popualates the Jtable with the Routing Table
        String[] columnNames ={"Network IP Address", "Next Hop"};
        String[] row1Data = {"135.46.56.0/22", "Interface 0"};
        String[] row2Data = {"135.46.60.0/22", "Interface 1"};
        String[] row3Data = {"192.53.40.0/23", "Router 1"};
        String[] row4Data = {"Default", "Router 2"};
        DefaultTableModel model = new DefaultTableModel(columnNames, 0);
        model.addRow(columnNames);
        model.addRow(row1Data);
        model.addRow(row2Data);
        model.addRow(row3Data);
        model.addRow(row4Data);

        RoutingTable.setModel(model);


        //Populates the List with the Suggested IP's
        DefaultListModel model1 = new DefaultListModel();
        model1.addElement("135.46.63.10");
        model1.addElement("135.46.57.14");
        model1.addElement("135.46.52.2");
        model1.addElement("192.53.40.7");
        model1.addElement("192.53.56.7");
        list1.setModel(model1);
    }

    public static void main(String[] args) {
        //Initiates the Frame for Use
        JFrame frame = new JFrame("RouteMyIP");
        frame.setContentPane(new RouteMyIP().GUIPanel);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }
}


