#!/bin/sh

rsync -aAXvP /* /mnt --exclude={/dev/*,/proc/*,/sys/*,/tmp/*,/run/*,/mnt/*,/media/*,/lost+found,/.gvfs}

cp -avT /run/archiso/bootmnt/arch/boot/$(uname -m)/vmlinuz /mnt/boot/vmlinuz-linux

sed -i 's/Storage=volatile/#Storage=auto/' /mnt/etc/systemd/journald.conf

rm /mnt/etc/udev/rules.d/81-dhcpcd.rules
rm /mnt/root/{.automated_script.sh,.zlogin}
rm -r /mnt/etc/systemd/system/{choose-mirror.service,pacman-init.service,etc-pacman.d-gnupg.mount,getty@tty1.service.d}
rm /mnt/etc/systemd/scripts/choose-mirror
rm /mnt/etc/sudoers.d/g_wheel
rm /mnt/etc/mkinitcpio-archiso.conf
rm -r /mnt/etc/initcpio

genfstab -L /mnt >> /mnt/etc/fstab && cat /mnt/etc/fstab

arch-chroot /mnt /bin/bash

umount /mnt -R