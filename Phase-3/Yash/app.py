from flask import Flask, render_template, jsonify, request, send_file
import sqlite3
import ctypes
import os

app = Flask(__name__)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATABASE = os.path.join(BASE_DIR, "railway.db")
DLL_PATH = os.path.join(BASE_DIR, "railway_backend.dll")

# C backend uses a relative SQLite path.
os.chdir(BASE_DIR)

STATUS = {
    0: "Cancelled",
    1: "Confirmed",
    2: "Waiting",
    3: "RAC"
}

COACHES = {
    0: "A1 - AC First Class",
    1: "B1 - AC 2 Tier",
    2: "C1 - AC 3 Tier",
    3: "S1 - Sleeper",
    4: "D1 - Chair Car"
}

PAYMENTS = {
    0: "Not Selected",
    1: "Cash",
    2: "UPI",
    3: "Debit Card",
    4: "Credit Card",
    5: "Net Banking"
}


# --------------------------------------------------
# C STRUCTURE: MUST MATCH passenger.h
# --------------------------------------------------

class Passenger(ctypes.Structure):
    _fields_ = [
        ("id", ctypes.c_int),
        ("pnr", ctypes.c_longlong),
        ("name", ctypes.c_char * 50),
        ("age", ctypes.c_int),
        ("priority", ctypes.c_int),
        ("seatNo", ctypes.c_int),
        ("status", ctypes.c_int),
        ("coachType", ctypes.c_int),
        ("ticketPrice", ctypes.c_float),
        ("paymentStatus", ctypes.c_int),
        ("paymentMethod", ctypes.c_int)
    ]


# --------------------------------------------------
# LOAD C/C++ DLL
# --------------------------------------------------

backend = None
backend_error = None
dll_directory = None


def initialize_backend():
    global backend, backend_error, dll_directory

    try:
        if not os.path.exists(DLL_PATH):
            raise FileNotFoundError("railway_backend.dll not found")

        # Add MSYS2 runtime DLL directory.
        runtime_path = r"C:\msys64-new\ucrt64\bin"

        if os.path.isdir(runtime_path):
            dll_directory = os.add_dll_directory(runtime_path)

        backend = ctypes.CDLL(DLL_PATH)

        # Function declarations
        backend.web_initialize.argtypes = []
        backend.web_initialize.restype = ctypes.c_int

        backend.web_register_passenger.argtypes = [
            ctypes.c_int,
            ctypes.c_char_p,
            ctypes.c_int,
            ctypes.c_int,
            ctypes.c_int
        ]
        backend.web_register_passenger.restype = ctypes.c_int

        backend.web_book_ticket.argtypes = [
            ctypes.c_int,
            ctypes.c_int
        ]
        backend.web_book_ticket.restype = ctypes.c_int

        backend.web_cancel_ticket.argtypes = [
            ctypes.c_longlong,
            ctypes.POINTER(ctypes.c_double)
        ]
        backend.web_cancel_ticket.restype = ctypes.c_int

        backend.web_get_passenger_count.argtypes = []
        backend.web_get_passenger_count.restype = ctypes.c_int

        backend.web_get_passenger.argtypes = [
            ctypes.c_int,
            ctypes.POINTER(Passenger)
        ]
        backend.web_get_passenger.restype = ctypes.c_int

        backend.web_find_pnr.argtypes = [
            ctypes.c_longlong,
            ctypes.POINTER(Passenger)
        ]
        backend.web_find_pnr.restype = ctypes.c_int

        backend.web_get_coach_info.argtypes = [
            ctypes.c_int,
            ctypes.POINTER(ctypes.c_int),
            ctypes.POINTER(ctypes.c_int),
            ctypes.POINTER(ctypes.c_int),
            ctypes.POINTER(ctypes.c_float)
        ]
        backend.web_get_coach_info.restype = ctypes.c_int

        backend.web_get_queue_count.argtypes = [
            ctypes.c_int
        ]
        backend.web_get_queue_count.restype = ctypes.c_int

        backend.web_get_queue_passenger.argtypes = [
            ctypes.c_int,
            ctypes.c_int,
            ctypes.POINTER(Passenger)
        ]
        backend.web_get_queue_passenger.restype = ctypes.c_int

        backend.web_generate_ticket.argtypes = [
            ctypes.c_longlong
        ]
        backend.web_generate_ticket.restype = ctypes.c_int

        result = backend.web_initialize()

        if not result:
            raise RuntimeError("C backend initialization failed")

        print("C/C++ backend initialized successfully.")

    except Exception as error:
        backend = None
        backend_error = str(error)
        print("BACKEND ERROR:", backend_error)


def require_backend():
    if backend is None:
        return jsonify({
            "success": False,
            "message": "Backend unavailable: " + str(backend_error)
        }), 500

    return None


# --------------------------------------------------
# SQLITE CONNECTION
# --------------------------------------------------

def get_db():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row
    return conn


def passenger_status(row):
    if row["pnr"] <= 0:
        return "Not Booked"

    return STATUS.get(row["status"], "Unknown")


def passenger_to_json(p):
    return {
        "id": p.id,
        "name": p.name.decode("utf-8", errors="replace"),
        "age": p.age,
        "priority": p.priority,
        "coach": COACHES.get(p.coachType, "Not Assigned"),
        "seat": p.seatNo,
        "price": round(p.ticketPrice, 2),
        "status": STATUS.get(p.status, "Unknown"),
        "pnr": p.pnr,
        "payment_status": "Paid" if p.paymentStatus == 1 else "Unpaid",
        "payment_method": PAYMENTS.get(p.paymentMethod, "Unknown")
    }


# --------------------------------------------------
# HOME PAGE
# --------------------------------------------------

@app.route("/")
def home():
    return render_template("index.html")


# --------------------------------------------------
# DASHBOARD
# --------------------------------------------------

@app.route("/api/dashboard")
def dashboard():

    conn = get_db()

    total = conn.execute(
        "SELECT COUNT(*) FROM passengers"
    ).fetchone()[0]

    confirmed = conn.execute(
        "SELECT COUNT(*) FROM passengers WHERE status=1 AND pnr>0"
    ).fetchone()[0]

    waiting = conn.execute(
        "SELECT COUNT(*) FROM passengers WHERE status=2 AND pnr>0"
    ).fetchone()[0]

    rac = conn.execute(
        "SELECT COUNT(*) FROM passengers WHERE status=3 AND pnr>0"
    ).fetchone()[0]

    cancelled = conn.execute(
        "SELECT COUNT(*) FROM passengers WHERE status=0 AND pnr>0"
    ).fetchone()[0]

    revenue = conn.execute("""
        SELECT COALESCE(SUM(ticket_price), 0)
        FROM passengers
        WHERE payment_status=1
          AND pnr>0
          AND status IN (1,2,3)
    """).fetchone()[0]

    conn.close()

    return jsonify({
        "total": total,
        "confirmed": confirmed,
        "waiting": waiting,
        "rac": rac,
        "cancelled": cancelled,
        "revenue": round(revenue, 2)
    })


# --------------------------------------------------
# PASSENGER LIST
# --------------------------------------------------

@app.route("/api/passengers")
def passengers():

    search = request.args.get("q", "").strip()

    conn = get_db()

    query = """
        SELECT id, name, age, priority, coach_type,
               seat_no, ticket_price, status, pnr,
               payment_status, payment_method
        FROM passengers
    """

    params = []

    if search:
        query += """
            WHERE name LIKE ?
               OR CAST(id AS TEXT) LIKE ?
               OR CAST(pnr AS TEXT) LIKE ?
        """

        term = f"%{search}%"
        params = [term, term, term]

    query += " ORDER BY id DESC"

    rows = conn.execute(query, params).fetchall()
    conn.close()

    result = []

    for row in rows:
        result.append({
            "id": row["id"],
            "name": row["name"],
            "age": row["age"],
            "priority": row["priority"],
            "coach": COACHES.get(row["coach_type"], "Not Assigned"),
            "seat": row["seat_no"],
            "price": row["ticket_price"],
            "status": passenger_status(row),
            "pnr": row["pnr"],
            "payment_status": (
                "Paid" if row["payment_status"] == 1 else "Unpaid"
            ),
            "payment_method": PAYMENTS.get(
                row["payment_method"], "Unknown"
            )
        })

    return jsonify(result)


# --------------------------------------------------
# REGISTER PASSENGER
# --------------------------------------------------

@app.route("/api/register", methods=["POST"])
def register_passenger():

    error = require_backend()
    if error:
        return error

    data = request.get_json(silent=True) or {}

    name = str(data.get("name", "")).strip()
    age = data.get("age")
    priority = data.get("priority", 2)
    coach = data.get("coach", 0)

    if not name or len(name.encode("utf-8")) > 49:
        return jsonify({
            "success": False,
            "message": "Enter a name of at most 49 bytes."
        }), 400

    try:
        age = int(age)
        priority = int(priority)
        coach = int(coach)
    except (ValueError, TypeError):
        return jsonify({
            "success": False,
            "message": "Invalid passenger details."
        }), 400

    if age < 1 or age > 120:
        return jsonify({
            "success": False,
            "message": "Enter a valid age."
        }), 400

    if priority not in (1, 2) or coach not in range(5):
        return jsonify({
            "success": False,
            "message": "Invalid priority or coach."
        }), 400

    conn = get_db()

    passenger_id = conn.execute(
        "SELECT COALESCE(MAX(id), 0) + 1 FROM passengers"
    ).fetchone()[0]

    conn.close()

    success = backend.web_register_passenger(
        passenger_id,
        name.encode("utf-8"),
        age,
        priority,
        coach
    )

    if not success:
        return jsonify({
            "success": False,
            "message": "Passenger registration failed."
        }), 400

    return jsonify({
        "success": True,
        "message": "Passenger registered successfully.",
        "passenger_id": passenger_id
    })


# --------------------------------------------------
# BOOK TICKET
# --------------------------------------------------

@app.route("/api/book", methods=["POST"])
def book_ticket():

    error = require_backend()
    if error:
        return error

    data = request.get_json(silent=True) or {}

    try:
        passenger_id = int(data.get("passenger_id"))
        payment_method = int(data.get("payment_method", 2))
    except (ValueError, TypeError):
        return jsonify({
            "success": False,
            "message": "Invalid passenger ID or payment method."
        }), 400

    if payment_method not in range(1, 6):
        return jsonify({
            "success": False,
            "message": "Invalid payment method."
        }), 400

    success = backend.web_book_ticket(
        passenger_id,
        payment_method
    )

    if not success:
        return jsonify({
            "success": False,
            "message": "Booking failed. Check passenger ID and booking status."
        }), 400

    return jsonify({
        "success": True,
        "message": "Booking request processed successfully."
    })


# --------------------------------------------------
# CANCEL TICKET
# --------------------------------------------------

@app.route("/api/cancel", methods=["POST"])
def cancel_ticket():

    error = require_backend()
    if error:
        return error

    data = request.get_json(silent=True) or {}

    try:
        pnr = int(data.get("pnr"))
    except (ValueError, TypeError):
        return jsonify({
            "success": False,
            "message": "Enter a valid PNR."
        }), 400

    refund = ctypes.c_double(0)

    success = backend.web_cancel_ticket(
        pnr,
        ctypes.byref(refund)
    )

    if not success:
        return jsonify({
            "success": False,
            "message": "Cancellation failed. Check the PNR."
        }), 400

    return jsonify({
        "success": True,
        "message": "Ticket cancelled successfully.",
        "refund": round(refund.value, 2)
    })


# --------------------------------------------------
# COACH AVAILABILITY
# --------------------------------------------------

@app.route("/api/coaches")
def coaches():

    error = require_backend()
    if error:
        return error

    result = []

    for coach_id, coach_name in COACHES.items():

        total = ctypes.c_int()
        booked = ctypes.c_int()
        available = ctypes.c_int()
        price = ctypes.c_float()

        success = backend.web_get_coach_info(
            coach_id,
            ctypes.byref(total),
            ctypes.byref(booked),
            ctypes.byref(available),
            ctypes.byref(price)
        )

        if success:
            result.append({
                "id": coach_id,
                "name": coach_name,
                "total": total.value,
                "booked": booked.value,
                "available": available.value,
                "price": price.value
            })

    return jsonify(result)


# --------------------------------------------------
# FIND PASSENGER BY PNR
# --------------------------------------------------

@app.route("/api/pnr/<int:pnr>")
def find_pnr(pnr):

    error = require_backend()
    if error:
        return error

    passenger = Passenger()

    success = backend.web_find_pnr(
        pnr,
        ctypes.byref(passenger)
    )

    if not success:
        return jsonify({
            "success": False,
            "message": "PNR not found."
        }), 404

    return jsonify({
        "success": True,
        "passenger": passenger_to_json(passenger)
    })


# --------------------------------------------------
# GENERATE TICKET
# --------------------------------------------------

@app.route("/api/ticket/<int:pnr>")
def generate_ticket(pnr):

    error = require_backend()
    if error:
        return error

    success = backend.web_generate_ticket(pnr)

    if not success:
        return jsonify({
            "success": False,
            "message": "Ticket generation failed."
        }), 400

    ticket_path = os.path.join(BASE_DIR, f"Ticket_{pnr}.txt")

    if not os.path.exists(ticket_path):
        return jsonify({
            "success": False,
            "message": "Ticket file was not created."
        }), 500

    return send_file(
        ticket_path,
        as_attachment=True,
        download_name=f"Ticket_{pnr}.txt"
    )


# --------------------------------------------------
# BOOKING HISTORY
# --------------------------------------------------

@app.route("/api/history")
def history():

    conn = get_db()

    rows = conn.execute("""
        SELECT history_id, passenger_id, pnr,
               event_type, status, coach_type,
               seat_no, ticket_price,
               payment_status, event_time
        FROM booking_history
        ORDER BY history_id DESC
        LIMIT 200
    """).fetchall()

    conn.close()

    result = []

    for row in rows:
        result.append({
            "history_id": row["history_id"],
            "passenger_id": row["passenger_id"],
            "pnr": row["pnr"],
            "event": row["event_type"],
            "status": STATUS.get(row["status"], "Unknown"),
            "coach": COACHES.get(row["coach_type"], "Not Assigned"),
            "seat": row["seat_no"],
            "price": row["ticket_price"],
            "payment": "Paid" if row["payment_status"] == 1 else "Unpaid",
            "time": row["event_time"]
        })

    return jsonify(result)


# --------------------------------------------------
# REPORTS
# --------------------------------------------------

@app.route("/api/reports")
def reports():

    conn = get_db()

    rows = conn.execute("""
        SELECT event_type, COUNT(*) AS total
        FROM booking_history
        GROUP BY event_type
    """).fetchall()

    result = {
        row["event_type"]: row["total"]
        for row in rows
    }

    conn.close()

    return jsonify(result)


# --------------------------------------------------
# START APPLICATION
# --------------------------------------------------

if __name__ == "__main__":

    if not os.path.exists(DATABASE):
        print("ERROR: railway.db not found!")

    else:
        print("Railway Reservation Web Interface")
        print("Database:", DATABASE)

        initialize_backend()

        app.run(
            host="127.0.0.1",
            port=5000,
            debug=False
        )