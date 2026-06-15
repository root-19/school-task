import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch

fig, ax = plt.subplots(figsize=(11, 8.5))
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.axis('off')

box_style = dict(boxstyle="round,pad=0.35", linewidth=1.5, edgecolor="#1f4e79", facecolor="#e8f1fb")
box_style_decision = dict(boxstyle="round,pad=0.35", linewidth=1.5, edgecolor="#7a2e70", facecolor="#f9ebfb")
box_style_action = dict(boxstyle="round,pad=0.35", linewidth=1.5, edgecolor="#2f6b3c", facecolor="#ecf8ef")

nodes = {
    "start": (0.5, 0.93, "Start"),
    "init": (0.5, 0.82, "Initialize System\n- Connect to SQLite DB\n- Create tables if missing"),
    "login": (0.5, 0.70, "Prompt Login (max 3 attempts)"),
    "fail": (0.15, 0.58, "3 failed attempts\n-> Return to main menu"),
    "role": (0.5, 0.56, "Check user role"),
    "user_menu": (0.2, 0.48, "USER MENU\n- View Sessions\n- View Available Slots\n- Enroll Student\n- View All Students"),
    "user_enroll": (0.2, 0.33, "Enroll Student Flow\n1. Enter student number (unique)\n2. Collect name, gender, age\n3. Pick session + modality from availability\n4. Insert student + decrement slots"),
    "user_loop": (0.2, 0.18, "Loop: after action return to USER menu"),
    "user_logout": (0.2, 0.07, "Logout -> back to login"),
    "admin_menu": (0.8, 0.48, "ADMIN MENU\n- Add / Update / Delete Sessions\n- Display Sessions\n- View / Remove Students"),
    "admin_manage": (0.8, 0.33, "Session Management\n• Validate inputs\n• Update slots\n• Persist changes"),
    "admin_students": (0.8, 0.20, "Student Oversight\n• Display all students\n• Remove student (restore slot)"),
    "admin_logout": (0.8, 0.07, "Logout -> back to login"),
}

for key, (x, y, text) in nodes.items():
    if key in {"start", "init", "login"}:
        style = box_style
    elif key == "role":
        style = box_style_decision
    elif key in {"admin_manage", "admin_students", "user_enroll"}:
        style = box_style_action
    else:
        style = box_style
    ax.text(x, y, text, ha='center', va='center', fontsize=10, bbox=style)

# Connections
arrows = [
    ("start", "init"),
    ("init", "login"),
    ("login", "fail"),
    ("login", "role"),
    ("role", "user_menu"),
    ("role", "admin_menu"),
    ("user_menu", "user_enroll"),
    ("user_enroll", "user_loop"),
    ("user_loop", "user_menu"),
    ("user_menu", "user_logout"),
    ("admin_menu", "admin_manage"),
    ("admin_manage", "admin_menu"),
    ("admin_menu", "admin_students"),
    ("admin_students", "admin_menu"),
    ("admin_menu", "admin_logout"),
]

for src, dst in arrows:
    x1, y1, _ = nodes[src]
    x2, y2, _ = nodes[dst]
    ax.annotate("", xy=(x2, y2+0.03 if y2 < y1 else y2-0.03), xytext=(x1, y1-0.03 if y1 > y2 else y1+0.03),
                arrowprops=dict(arrowstyle="->", linewidth=1.3, color="#1f4e79"))

# Labels for decision branches
ax.text(0.35, 0.53, "Role = USER", fontsize=9, color="#1f4e79")
ax.text(0.65, 0.53, "Role = ADMIN", fontsize=9, color="#1f4e79")
ax.text(0.32, 0.63, "Invalid credentials", fontsize=9, color="#1f4e79")

ax.text(0.5, 0.02, "System flow continues until user exits application", fontsize=9, ha='center', color="#444444")

fig.savefig("docs/system_flowchart.pdf", bbox_inches="tight")
print("Flowchart exported to docs/system_flowchart.pdf")
