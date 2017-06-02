package viking.viking;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

public class MainActivity extends AppCompatActivity {

    TextView title;
    TextView lot0;
    TextView lot1;
    TextView lot2;
    TextView lot3;
    TextView log;

    int temp0 = 0;
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        title = (TextView) findViewById(R.id.title);
        lot0 = (TextView) findViewById(R.id.lot0);
        lot1 = (TextView) findViewById(R.id.lot1);
        lot2 = (TextView) findViewById(R.id.lot2);
        lot3 = (TextView) findViewById(R.id.lot3);
        log = (TextView) findViewById(R.id.log);

        Timer myTimer = new Timer();
        myTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                getData task = new getData();
                task.execute();
            }
        }, 0, 10000);
    }

    void setRed(int i) {
        switch (i) {
            case 0: {
                lot0.setBackgroundColor(0xaaff0000);
                break;
            }
            case 1: {
                lot1.setBackgroundColor(0xaaff0000);
                break;
            }
            case 2: {
                lot2.setBackgroundColor(0xaaff0000);
                break;
            }
            case 3: {
                lot3.setBackgroundColor(0xaaff0000);
                break;
            }
            default: {
                lot0.setBackgroundColor(0xaa00ff00);
                lot1.setBackgroundColor(0xaa00ff00);
                lot2.setBackgroundColor(0xaa00ff00);
                lot3.setBackgroundColor(0xaa00ff00);
            }
        }
    }

    void setGreen(int i) {
        switch (i) {
            case 0: {
                lot0.setBackgroundColor(0xaa00ff00);
                break;
            }
            case 1: {
                lot1.setBackgroundColor(0xaa00ff00);
                break;
            }
            case 2: {
                lot2.setBackgroundColor(0xaa00ff00);
                break;
            }
            case 3: {
                lot3.setBackgroundColor(0xaa00ff00);
                break;
            }
            default: {
                lot0.setBackgroundColor(0xaaff0000);
                lot1.setBackgroundColor(0xaaff0000);
                lot2.setBackgroundColor(0xaaff0000);
                lot3.setBackgroundColor(0xaaff0000);
            }
        }
    }

    void checkSlot(int slot, int ldr) {
        if (ldr < 500) {
            if (slot == 0) {
                if (temp0 == 1) {
                    setGreen(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 4 Available" + s);
                    temp0 = 0;
                }
            }
            if (slot == 1) {
                if (temp1 == 1) {
                    setGreen(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 3 Available" + s);
                    temp1 = 0;
                }
            }
            if (slot == 2) {
                if (temp2 == 1) {
                    setGreen(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 2 Available" + s);
                    temp2 = 0;
                }
            }
            if (slot == 3) {
                if (temp3 == 1) {
                    setGreen(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 1 Available" + s);
                    temp3 = 0;
                }
            }

        } else if (ldr > 500) {

            if (slot == 0) {
                if (temp0 == 0) {
                    setRed(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 4 Unavailable" + s);
                    temp0 = 1;
                }
            }
            if (slot == 1) {
                if (temp1 == 0) {
                    setRed(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 3 Unavailable" + s);
                    temp1 = 1;
                }
            }
            if (slot == 2) {
                if (temp2 == 0) {
                    setRed(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 2 Unavailable" + s);
                    temp2 = 1;
                }
            }
            if (slot == 3) {
                if (temp3 == 0) {
                    setRed(slot);
                    String s = log.getText().toString();
                    SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                    String currentDateandTime = sdf.format(new Date());
                    log.setText("\n" + currentDateandTime + " Lot 1 Unavailable" + s);
                    temp3 = 1;
                }
            }
        }
    }

    class getData extends AsyncTask<Void, Vector<String>, Vector<String>> {

        @Override
        protected Vector<String> doInBackground(Void... params) {

            Document doc = null;
            try {
                doc = Jsoup.connect("http://192.168.4.1").get();
            } catch (IOException e) {
                e.printStackTrace();
            }

            String ldr0 = doc.getElementById("ldr1").text();
            String ldr1 = doc.getElementById("ldr2").text();
            String ldr2 = doc.getElementById("ldr3").text();
            String ldr3 = doc.getElementById("ldr4").text();

            Vector<String> v = new Vector<String>();
            v.add(ldr0);
            v.add(ldr1);
            v.add(ldr2);
            v.add(ldr3);

            return v;
        }

        @Override
        protected void onPostExecute(Vector<String> v) {
            super.onPostExecute(v);
            int i = 0;
            while (i < 4) {
                checkSlot(i, Integer.parseInt(v.get(i)));
                i++;
            }
            i = 1;
        }
    }
}