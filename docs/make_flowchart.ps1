$lines = @(
    "BT",
    "/F1 12 Tf",
    "12 TL",
    "72 760 Td (School Management System Flowchart) Tj",
    "T*",
    "(----------------------------------------------) Tj",
    "T*",
    "([Start]) Tj",
    "T*",
    "(  |) Tj",
    "T*",
    "(  v) Tj",
    "T*",
    "(Initialize system: connect DB, ensure tables) Tj",
    "T*",
    "(  |) Tj",
    "T*",
    "(  v) Tj",
    "T*",
    "(Login (max 3 attempts)) Tj",
    "T*",
    "(  |-- fail --> Return to start) Tj",
    "T*",
    "(  v) Tj",
    "T*",
    "(Role check) Tj",
    "T*",
    "(  |--- USER ---> User menu: view sessions, view slots) Tj",
    "T*",
    "(  |              enroll student (unique number), view students) Tj",
    "T*",
    "(  |                 |) Tj",
    "T*",
    "(  |                 v) Tj",
    "T*",
    "(  |            After action -> USER menu or logout) Tj",
    "T*",
    "(  |) Tj",
    "T*",
    "(  |--- ADMIN --> Admin menu: add/update/delete sessions) Tj",
    "T*",
    "(                   manage students (view/remove)) Tj",
    "T*",
    "(                   After action -> ADMIN menu or logout) Tj",
    "T*",
    "(End loop when application exits) Tj",
    "ET"
)

$stream = ($lines -join "`n") + "`n"
$encoding = [System.Text.Encoding]::ASCII

$objects = @()
$objects += "1 0 obj`n<< /Type /Catalog /Pages 2 0 R >>`nendobj`n"
$objects += "2 0 obj`n<< /Type /Pages /Kids [3 0 R] /Count 1 >>`nendobj`n"
$objects += "3 0 obj`n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>`nendobj`n"
$length = $encoding.GetByteCount($stream)
$objects += "4 0 obj`n<< /Length $length >>`nstream`n$stream`nendstream`nendobj`n"
$objects += "5 0 obj`n<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>`nendobj`n"

$ms = New-Object System.IO.MemoryStream
$writer = New-Object System.IO.StreamWriter($ms, $encoding)
$writer.NewLine = "`n"

$writer.Write("%PDF-1.4`n")
$writer.Flush()

$offsets = @()
foreach ($obj in $objects) {
    $offsets += $ms.Position
    $writer.Write($obj)
    $writer.Flush()
}

$startxref = $ms.Position
$count = $objects.Count + 1
$writer.Write("xref`n0 $count`n")
$writer.Write("0000000000 65535 f `n")
for ($i = 0; $i -lt $objects.Count; $i++) {
    $writer.Write( ("{0:D10} 00000 n `n" -f $offsets[$i]) )
}
$writer.Write("trailer`n<< /Size $count /Root 1 0 R >>`nstartxref`n$startxref`n%%EOF")
$writer.Flush()

[System.IO.File]::WriteAllBytes("C:/Users/admin/Desktop/ticket-booking/docs/system_flowchart.pdf", $ms.ToArray())
Write-Host "Flowchart PDF regenerated at docs/system_flowchart.pdf"
