E:
CD \HOPPY
recurse purge
CD \
del HOPPY.ZIP
F:\PROGRA~1\PKWARE\PKZIPC\PKZIPC -add -dir -rec HOPPY.ZIP \HOPPY\*.*
F:
cd "\TDL5\simstructure Website"
copy e:\HOPPY.ZIP
copy e:\HOPPY\upload.ftp
ftp -s:upload.ftp www.simstructure.hare.com

