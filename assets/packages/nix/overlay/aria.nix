self: super:
{
  aria = super.aria.overrideAttrs (oldAttrs: {
    configureFlags = oldAttrs.configureFlags ++ [ "--enable-libaria2" ];
  });
}
