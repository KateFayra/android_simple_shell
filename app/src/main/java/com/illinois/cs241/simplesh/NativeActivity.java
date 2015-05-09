package com.illinois.cs241.simplesh;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.TextView;


public class NativeActivity extends ActionBarActivity {

    NativeClass nClass = new NativeClass();
    String commandText = "";
    String userName = "";
    String hostname = "";
    String dirName = "";
    TextView textBox;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        userName = new String(nClass.getUserName());
        hostname = new String(nClass.getHostname());
        dirName = new String(nClass.getcwd());
        textBox = (TextView)findViewById(R.id.textView);
        textBox.setText("[" + userName + "@" + hostname + " " + dirName + "]$ ");
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        if(keyCode == KeyEvent.KEYCODE_ENTER){
            System.out.println("Return pressed!");
            textBox.setText(textBox.getText() + "\n");

            String returnedText;
            if(commandText.startsWith("cd ")){
                String path = commandText.substring(3);
                if(nClass.chdir(path) == 0) {
                    returnedText = "Successfully changed directory to: " + path;
                    dirName = new String(nClass.getcwd());
                } else {
                    returnedText = "Changing directory to: " + path + " failed.";
                }
            } else {
                returnedText = new String(nClass.runCommand(commandText));
            }
            System.out.println(returnedText);
            textBox.setText(textBox.getText() + returnedText + "\n[" + userName + "@" + hostname + " " + dirName + "]$ ");
            commandText = "";

            /*
            //@todo: Scrolling does not scroll to the bottom correctly due to the text on the GUI (and the scrollview's length) being refreshed after this function is returned.
            ScrollView scroll = (ScrollView) this.findViewById(R.id.scrollView);
            scroll.fullScroll(ScrollView.FOCUS_DOWN);
            */

        } else if(keyCode == KeyEvent.KEYCODE_DEL) {
            if(commandText.length() > 0) {
                commandText = commandText.substring(0, commandText.length() - 1);

                String newText = "" + textBox.getText();
                newText = newText.substring(0, newText.length() - 1);
                textBox.setText(newText);
            }
        } else {
            commandText += Character.toString((char) event.getUnicodeChar());
            System.out.println("Key " + (char) event.getUnicodeChar() + " pressed!");
            System.out.println("commandText: " + commandText);
            textBox.setText(textBox.getText() + Character.toString((char) event.getUnicodeChar()));
        }

        return true;
    }
}

