"""! @brief Plot Data From SQL DataBase"""

import sqlite3
from datetime import datetime

from matplotlib import dates
from matplotlib import pyplot as plt


with sqlite3.connect("db/iot.db") as con:
    cur = con.cursor()
    cur.execute(
        """
        SELECT recorded_at, data_value
        FROM Readings;
        """
    )
    dates_list: list[datetime] = []
    values: list[float] = []
    for row in cur:
        date, value = row
        date_obj = datetime.strptime(date, "%Y-%m-%d %H:%M:%S")
        dates_list.append(date_obj) 
        values.append(value)
        
    fig, ax = plt.subplots()
    ax.plot(dates.date2num(dates_list), values)
    myFmt = dates.DateFormatter('%m/%d %H:%M')
    plt.gca().xaxis.set_major_formatter(myFmt)
    # plt.plot(dates, values)
    plt.gcf().autofmt_xdate()
    plt.autoscale()
    plt.show()
