import sys
import smtplib
from email.message import EmailMessage
from fpdf import FPDF
import os

# Get filename from command-line argument
if len(sys.argv) < 2:
    print("Error: No filename provided.")
    sys.exit(1)

receipt_txt = sys.argv[1]
receipt_pdf = os.path.splitext(receipt_txt)[0] + ".pdf"  # replace .txt with .pdf

# Convert .txt file to .pdf
pdf = FPDF()
pdf.add_page()
pdf.set_font("Arial", size=12)

try:
    with open(receipt_txt, "r") as file:
        for line in file:
            pdf.cell(200, 10, txt=line.strip(), ln=True)
except FileNotFoundError:
    print(f"Error: File '{receipt_txt}' not found.")
    sys.exit(1)

pdf.output(receipt_pdf)

# Compose the email
msg = EmailMessage()
msg["Subject"] = "Your Receipt PDF"
msg["From"] = "jlolpezolguin@gmail.com"
msg["To"] = "sanamghale.ca@gmail.com"
msg.set_content("Hello, please find the attached receipt as a PDF.\n\nSent via Python script.")

# Attach the PDF
with open(receipt_pdf, "rb") as f:
    file_data = f.read()
    msg.add_attachment(file_data, maintype="application", subtype="pdf", filename=os.path.basename(receipt_pdf))

# Send the email using Gmail SMTP
smtp_server = "smtp.gmail.com"
port = 587
# Gmail account credentials (used for testing purposes)
username = "jlolpezolguin@gmail.com"
password = "lscfiymldugabguh"  #  Use an app-specific password if 2FA is enabled

with smtplib.SMTP(smtp_server, port) as server:
    server.starttls()
    server.login(username, password)
    server.send_message(msg)

print("Email sent successfully.")
