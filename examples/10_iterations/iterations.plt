# -----------------------------------------------------------------------------
# gnuplot script (version 4.6 or higher)
# plot relative errors and number of iterations for libkes solver method
# Bazso Akos
# V1.0, 12 May 2019 -- initial script
# V1.1, 20 Nov 2019 -- modified order of columns
# -----------------------------------------------------------------------------

reset

# TODO change input/output file name here
filename="mikkola"

# -----------------------------------------------------------------------------

# --- terminal settings ---
set term pngcairo enh size 1000,1200 font ",20"
set out filename.".png"

# -----------------------------------------------------------------------------

# --- general settings ---
set border lt -1 lw 2
unset key
set pm3d map interp 0,0
set tics out nomir scale 1.5

# -----------------------------------------------------------------------------

# --- axes settings ---
set xlabel "eccentric anomaly/{/Symbol p} [rad]"
set xrange [0:pi]
set xtics 0.2*pi
set mxtic 2
set format x "%.1P"

set ylabel "eccentricity"
set yrange [0:1]
set ytics 0.2
set mytic 2
set format y "%.1f"

set cbtics 1 in mir scale 2

# -----------------------------------------------------------------------------

set multi layout 2,1

# --- plot #1 ---
set cblabel "log_{10} relative error"
set title "(a) relative error"
sp filename.".dat" u 3:2:(log10($5)) not

# --- plot #2 ---
set cblabel "number of iterations"
#set cbtics 10
set title "(b) number of iterations"
sp filename.".dat" u 3:2:6 not

unset multi

set out

# -----------------------------------------------------------------------------
