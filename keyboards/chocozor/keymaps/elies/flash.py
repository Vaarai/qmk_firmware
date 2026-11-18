"""Build this keymap and flash it to the half plugged in over USB.

The board reboots into its bootloader on a Raw HID command (hud.c); a board
without it, or one that doesn't answer, is asked to be put in flash mode by
hand. Run from anywhere: python3 flash.py [--no-build]
"""

import glob
import os
import subprocess
import sys
import time

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../.."))
TARGET = "chocozor/v1_3:elies"
UF2 = os.path.join(ROOT, "chocozor_v1_3_elies.uf2")
HID_ID = "HID_ID=0003:0000FEED:00000130"
HUD_BOOT = 4


def raw_hid():
    for node in sorted(glob.glob("/sys/class/hidraw/hidraw*")):
        try:
            with open(node + "/device/uevent") as f:
                if HID_ID not in f.read().split():
                    continue
            with open(node + "/device/report_descriptor", "rb") as f:
                if f.read(3) == b"\x06\x60\xff":
                    return "/dev/" + os.path.basename(node)
        except OSError:
            continue
    return None


def bootloader_drive():
    out = subprocess.run(["lsblk", "-rno", "NAME,LABEL"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        name, _, label = line.partition(" ")
        if label == "RPI-RP2":
            return "/dev/" + name
    return None


def wait(what, check, timeout, fatal=True):
    end = time.monotonic() + timeout
    while time.monotonic() < end:
        found = check()
        if found:
            return found
        time.sleep(0.25)
    if fatal:
        sys.exit(f"flash: timed out waiting for {what}")
    return None


def main():
    if "--no-build" not in sys.argv:
        build = subprocess.run(
            ["nix", "shell", "nixpkgs#qmk", "nixpkgs#gcc-arm-embedded", "-c", "make", TARGET],
            cwd=ROOT, capture_output=True, text=True)
        if build.returncode != 0 or "[ERRORS]" in build.stdout:
            sys.stdout.write(build.stdout[-4000:])
            sys.stderr.write(build.stderr[-4000:])
            sys.exit("flash: build failed")
        print("flash: built", os.path.relpath(UF2, ROOT), flush=True)

    drive = bootloader_drive()
    if not drive:
        hid = raw_hid()
        if hid:
            try:
                fd = os.open(hid, os.O_WRONLY)
                os.write(fd, bytes([0, HUD_BOOT, *b"BOOT"]).ljust(33, b"\0"))
                os.close(fd)
                print("flash: asked the board to enter its bootloader", flush=True)
            except OSError as e:
                print(f"flash: could not reach the board ({e})")
            drive = wait("the RPI-RP2 drive", bootloader_drive, 10, fatal=False)
        if not drive:
            print("flash: put the half in flash mode", flush=True)
            drive = wait("the RPI-RP2 drive", bootloader_drive, 120)

    subprocess.run(["udisksctl", "mount", "-b", drive], capture_output=True)
    mount = wait("the drive to mount",
                 lambda: subprocess.run(["lsblk", "-no", "MOUNTPOINT", drive],
                                        capture_output=True, text=True).stdout.strip(), 15)
    with open(os.path.join(mount, "INFO_UF2.TXT")) as f:
        if "RPI-RP2" not in f.read():
            sys.exit(f"flash: {mount} is not an RP2040 bootloader")
    with open(UF2, "rb") as src, open(os.path.join(mount, os.path.basename(UF2)), "wb") as dst:
        dst.write(src.read())
        dst.flush()
        os.fsync(dst.fileno())
    print("flash: copied, waiting for the board")
    wait("the board to come back", raw_hid, 30)
    print("flash: done")


if __name__ == "__main__":
    main()
