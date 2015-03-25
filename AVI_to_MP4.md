# Comando #

Comando para pasar videos del formato AVI a MP4.

  * ffmpeg -i cam2.avi -sameq -vcodec libx264 -acodec libfaac cam2.mp4

Para editar el video y sacar partes.

  * ffmpeg -i cam2.avi -sameq -vcodec libx264 -acodec libfaac -ss 00:00:06 -t 00:00:13  cam2.mp4

Donde:
  * **cam2.avi** es el video original y
  * **cam1.mp4** es el video de salida.